#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "util.h"

#define LINE_MAX_LEN 1024

static char* strip(char* line) {
    while (isspace(*line)) line++;
    char* end = line + strlen(line) - 1;
    while (end > line && isspace(*end)) *end-- = '\0';
    return line;
}

int parse_emberfile(const char* filename, EmberBuildConfig* config) {
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    char line[LINE_MAX_LEN];
    char current_section[32] = "";

    while (fgets(line, sizeof(line), file)) {
        char* stripped = strip(line);
        if (stripped[0] == ';' || stripped[0] == '\0') continue;

        if (stripped[0] == '.' && strchr(stripped, '{')) {
            sscanf(stripped, ".%31[^ {]{", current_section);
            continue;
        }

        if (stripped[0] == '}') {
            current_section[0] = '\0';
            continue;
        }

        if (strcmp(current_section, "target") == 0) {
            config->target = strdup(stripped);
        } else if (strcmp(current_section, "out") == 0) {
            config->out_dir = strdup(stripped);
        } else if (strcmp(current_section, "languages") == 0) {
            config->language = strdup(stripped);
        } else if (strcmp(current_section, "src") == 0) {
            if (config->src_count < MAX_FILES)
                config->srcs[config->src_count++] = strdup(stripped);
        } else if (strcmp(current_section, "flags") == 0) {
            if (config->flag_count < MAX_FLAGS)
                config->flags[config->flag_count++] = strdup(stripped);
        } else if (strcmp(current_section, "pre") == 0) {
            config->pre_cmd = strdup(stripped);
        } else if (strcmp(current_section, "post") == 0) {
            config->post_cmd = strdup(stripped);
        } else if (strcmp(current_section, "include") == 0) {
            config->includes[config->include_count++] = strdup(stripped);
        } else if (strcmp(current_section, "libs") == 0) {
            config->libs[config->lib_count++] = strdup(stripped);
        } else if (strcmp(current_section, "task") == 0) {
            char* colon = strchr(stripped, ':');
            if (colon) {
                *colon = '\0';
                char* name = trim(stripped);
                char* cmd = trim(colon + 1);

                config->task_names[config->task_count] = strdup(name);
                config->task_cmds[config->task_count] = strdup(cmd);
                config->task_count++;
            }
        }
    }

    fclose(file);
    return 0;
}