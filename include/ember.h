#ifndef EMBER_H
#define EMBER_H

#define MAX_FILES 128
#define MAX_FLAGS 32
#define MAX_CMD_LENGTH 512
#define MAX_INCLUDES 10
#define MAX_LIBS 10
#define MAX_TASKS 10

typedef struct {
    char* target;
    char* out_dir;
    char* language;
    char* srcs[MAX_FILES];
    int src_count;
    char* flags[MAX_FLAGS];
    int flag_count;
    char* pre_cmd;
    char* post_cmd;
    char* includes[MAX_INCLUDES];
    int include_count;
    char* libs[MAX_LIBS];
    int lib_count;
    char* task_names[MAX_TASKS];
    char* task_cmds[MAX_TASKS];
    int task_count;

} EmberBuildConfig;

#endif