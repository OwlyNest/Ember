#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "ember.h"
#include "parser.h"
#include "build.h"
#include "compilers.h"

int main(int argc, char *argv[]) {
    EmberBuildConfig config = {0};

    if (parse_emberfile("Emberfile", &config) != 0) {
        fprintf(stderr, "[X] Emberfile failed to parse. Try again, maybe better.\n");
        return 1;
    }   
    if (argc >= 2) {
        const char* task = argv[1];
        for (int i = 0; i < config.task_count; i++) {
            if (strcmp(task, config.task_names[i]) == 0) {
                printf("[T]  Running task '%s': %s\n", task, config.task_cmds[i]);
                int result = system(config.task_cmds[i]);
                return result;
            }
        }
        fprintf(stderr, "[X] Unknown task: %s\n", task);
        return 1;
    }    


    run_pre_cmd(&config);
    compile_and_link(&config);
    run_post_cmd(&config);

    return 0;
}