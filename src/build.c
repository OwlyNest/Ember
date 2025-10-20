#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "build.h"

int run_pre_cmd(const EmberBuildConfig* config) {
    if (config->pre_cmd) {
        return system(config->pre_cmd);
    }
    return 0;
}

int run_post_cmd(const EmberBuildConfig* config) {
    if (config->post_cmd) {
        return system(config->post_cmd);
    }
    return 0;
}