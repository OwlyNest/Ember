#include "compilers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define EXT_C      ".c"
#define EXT_FORTRAN ".f90"

#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_CYAN    "\x1b[36m"

static const char* compiler_for(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return "gcc";  // fallback

    if (strcmp(ext, EXT_FORTRAN) == 0)
        return "gfortran";
    else if (strcmp(ext, EXT_C) == 0)
        return "gcc";

    return "gcc"; // default fallback
}

static const char* determine_linker(const EmberBuildConfig* config) {
    for (int i = 0; i < config->src_count; i++) {
        if (strstr(config->srcs[i], EXT_FORTRAN)) {
            return "gfortran";
        }
    }
    return "gcc";
}

static int is_up_to_date(const char* src, const char* obj) {
    struct stat src_stat, obj_stat;
    if (stat(src, &src_stat) != 0 || stat(obj, &obj_stat) != 0)
        return 0;
    return difftime(src_stat.st_mtime, obj_stat.st_mtime) <= 0;
}

int compile_and_link(const EmberBuildConfig* config) {
    struct stat st = {0};
    if (stat(config->out_dir, &st) == -1) {
        printf(COLOR_YELLOW "[D] Creating directory: %s\n" COLOR_RESET, config->out_dir);
        char mkdir_cmd[256];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p %s", config->out_dir);
        system(mkdir_cmd);
    }

    char* objs[config->src_count];
    char cmd[1024];

    char all_flags[256] = "";
    for (int i = 0; i < config->flag_count; i++) {
        strcat(all_flags, config->flags[i]);
        strcat(all_flags, " ");
    }

    char include_flags[256] = "";
    for (int i = 0; i < config->include_count; i++) {
        strcat(include_flags, "-I");
        strcat(include_flags, config->includes[i]);
        strcat(include_flags, " ");
    }

    // === Compile sources ===
    for (int i = 0; i < config->src_count; i++) {
        const char* src = config->srcs[i];
        const char* compiler = compiler_for(src);

        const char* slash = strrchr(src, '/');
        const char* filename = slash ? slash + 1 : src;

        const char* ext = strrchr(filename, '.');
        int name_len = ext ? (int)(ext - filename) : strlen(filename);

        char obj_path[256];
        snprintf(obj_path, sizeof(obj_path), "%s/%.*s.o", config->out_dir, name_len, filename);
        objs[i] = strdup(obj_path);

        if (is_up_to_date(src, obj_path)) {
            printf(COLOR_GREEN "[S] %s (up-to-date)\n" COLOR_RESET, src);
            continue;
        }

        snprintf(cmd, sizeof(cmd), "%s -c %s -o %s %s %s", compiler, src, obj_path, include_flags, all_flags);
        printf(COLOR_YELLOW "[C] %s\n" COLOR_RESET, cmd);
        if (system(cmd) != 0) {
            fprintf(stderr, COLOR_RED "[X] Compilation failed: %s\n" COLOR_RESET, src);
            return 1;
        }
    }

    // === Link ===
    const char* linker = determine_linker(config);

    char output_path[256];
    snprintf(output_path, sizeof(output_path), "%s/%s", config->out_dir, config->target);

    char lib_flags[256] = "";
    for (int i = 0; i < config->lib_count; i++) {
        strcat(lib_flags, config->libs[i]);
        strcat(lib_flags, " ");
    }

    snprintf(cmd, sizeof(cmd), "%s", linker);
    for (int i = 0; i < config->src_count; i++) {
        strcat(cmd, " ");
        strcat(cmd, objs[i]);
    }
    strcat(cmd, " ");
    strcat(cmd, lib_flags);
    strcat(cmd, " -o ");
    strcat(cmd, output_path);

    printf(COLOR_CYAN "[L] %s\n" COLOR_RESET, cmd);
    if (system(cmd) != 0) {
        fprintf(stderr, COLOR_RED "[X] Linking failed.\n" COLOR_RESET);
        return 1;
    }

    printf(COLOR_GREEN "[B] Build successful: %s\n" COLOR_RESET, output_path);
    return 0;
}