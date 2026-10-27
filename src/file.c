#include <stdio.h>
#include <string.h>
#include "file.h"

static void terminate_strings(Registryfil *reg) {
    for (int i = 0; i < MAX_VEHICLES; i++) {
        reg->vehicles[i].type[TEXT_LEN - 1] = '\0';
        reg->vehicles[i].brand[TEXT_LEN - 1] = '\0';
        reg->vehicles[i].plate[TEXT_LEN  - 1] = '\0';
        reg->vehicles[i].owner.name[TEXT_LEN - 1]  = '\0';
    } 
}

filestatus file_load(Registryfil *reg, const char *path) {
    FILE *f = fopen(path, "rb");
    size_t read;

    if (f == NULL) {
        return file_load_FAILED;
    }

    read = fread(reg, sizeof *reg, 1, f);
    fclose(f);

    if (read != 1) {
        return file_load_FAILED;
    }

    terminate_strings(reg);
    return file_load_OK;
}

filestatus file_save(const Registryfil *reg, const char *path) {
    FILE *f = fopen(path, "wb");
    size_t written;

    if (f == NULL) {
        return file_save_FAILED;
    }

    written = fwrite(reg, sizeof *reg, 1, f);
    
    if (fclose(f) != 0 || written != 1) {
        return file_save_FAILED;
    }

    return file_SAVE;
}