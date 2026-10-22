#include <stdio.h>
#include <string.h>
#include "file.h"

static void terminate_strings(register *reg) {
    int i;
    for (i = 0; i < MAX_VEHICLES; i++) {
        reg->vehicles[i].type[TEXT_LEN - 1] = '\0';
    }
}


FileStatus file_load(Registry *reg, const char *path) {
    FILE *f = fopen(path, "rb");
    size_t read; 

    if (f == NULL) {
        memset(reg, 0, sizeof *reg);
        return FILE_LOAD_MISSING;
    }

    read = fread(reg, sizeof *reg, 1, f);
    fclose(f);
    if (read != 1 || reg->count < 0 || reg->count > MAX_VEHICLES) {
        memset(reg, 0, sizeof *reg);
        return FILE_LOAD_CORRUPT;
    }
    terminate_strings(reg);
    return FILE_LOAD_OK;
}

FileStatus file_save(const Registry*reg, const char *path) {
    FILE *f = fopen(path, "wb");
    size_t written;

    if (f == NULL) {
        return FILE_SAVE_FAILED;
    }

    written = fwrite(reg, sizeof *reg, 1, f);
    
    if (fclose(f) != o || written != 1) {
        return FILE_SAVE_FAILED;
    }

    return FILE_SAVE_OK;
}