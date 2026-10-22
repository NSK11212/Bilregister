#ifndef FILE_H
#define FILE_H

#define MAX_VEHICLES 10
#define TEXT_LEN 32
#define REGISTRY_FILE "registry.dat"

typedef struct {
    char name[TEXT_LEN];
    int age;
} Person;

typedef struct {
    char type[TEXT_LEN];
    char brand[TEXT_LEN];
    char plate[TEXT_LEN];
    Person owner;
} Vehicle;

typedef struct {
    Vehicle vehicles[MAX_VEHICLES];
    int count; 
} Registry;

typedef enum {
    FILE_LOAD_OK,
    FILE_LOAD_MISSING,
    FILE_LOAD_CORRUPT,
    FILE_SAVE_OK,
    FILE_SAVE_FAILED
} FileStatus;

FileStatus file_load(Registry *reg, const char *path);
FileStatus file_save(const Registry *reg, const char *path);

#endif