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
} Registryfil;

typedef enum {
    file_load_OK,
    file_load_FAILED,
    file_SAVE,
    file_save_FAILED
} filestatus;

filestatus file_load(Registryfil *reg, const char *path);
filestatus file_save(const Registryfil *reg, const char *path);

#endif