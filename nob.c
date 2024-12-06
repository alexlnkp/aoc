#define NOB_IMPLEMENTATION
#include "nob.h"

#define CC "gcc"

#define SRC_FOLDER "src"
#define OUT_FOLDER "out"

#define NUM_TASKS 6

void compile_all_tasks(const char* defs) {
    char* out_path = alloca(64);
    char* src_path = alloca(64);

    for (int i = 1; i <= NUM_TASKS; ++i) {
        sprintf(out_path, "%s/%d", OUT_FOLDER, i);
        sprintf(src_path, "%s/%d.c", SRC_FOLDER, i);
        Nob_Cmd cmd = {0};

        nob_cmd_append(&cmd, CC, "-Wall", "-Wextra", "-o", out_path, "-O3", "-Iinclude");
        if (defs) nob_cmd_append(&cmd, defs);
        nob_cmd_append(&cmd, src_path);
        nob_cmd_run_sync(cmd);
    }
}

int main(int argc, char* argv[]) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_mkdir_if_not_exists(OUT_FOLDER);

    if (argc > 1) compile_all_tasks("-DBENCHMARK");
    else compile_all_tasks(NULL);

    return 0;
}
