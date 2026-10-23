#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file.h"

#define MIN_AGE 18
#define MAX_AGE 100
#define RANDOM_MIN_AGE 18
#define RANDOM_MAX_AGE 90
#define PLATE_LETTERS 3
#define PLATE_DIGITS 3
#define DIGIT_BASE 16
#define LINE_LEN 64
#define COL_GAP 2
#define FIRST_POS 1

typedef enum {
    MENU_QUIT,
    MENU_ADD,
    MENU_REMOVE,
    MENU_SORT,
    MENU_SHOW,
    MENU_SHOW_ALL,
    MENU_RANDOM,
    MENU_LAST = MENU_RANDOM
} MenuChoice;

typedef enum {
    REG_OK,
    REG_FULL,
    REG_OGILTIGT_POSITION,
    REG_TOM_POSITION,
    REG_DUPLICATE_PLATE
} RegStatus;

typedef enum {
    INPUT_OK,
    INPUT_OGILTIGT,
    INPUT_END_OF_FILE
} InputStatus;

static const char *const RANDOM_BRANDS[] = {" Audi", "BMW", "Ford", "Honda", "Kia", "Peugeot", "Renault", "Saab", "Tesla", "Toyota", "Volkswagen", "Volvo"};
static const char *const RANDOM_TYPES[] = {"SUV", "Sedan", "Kupe", "Van", "Minivan", "Pickup", "Convertible", "Crossover"};
static const char *const RANDOM_FIRST_NAME = {"Anna"};
static const char *const RANDOM_LAST_NAME = {"Andersson"};

static const char PLATE_ALPHABET[] ="ABCDEFGHJKLMNOPRSTUWXYZ";

// safe input för lång sträng och newlines
static InputStatus read_line(char *buf, size_t size) {
    size_t len;
    int c;

    if (fgets(buf, (int)size, stdin) == NULL) return INPUT_END_OF_FILE;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') { buf[len - 1] = '\0'; return INPUT_OK; }
    if (feof(stdin)) return INPUT_OK;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return INPUT_OGILTIGT;
}

static int read_text(const char *prompt, char *buf, size_t size) {
    for (;;) {
        InputStatus status;
        printf("%s", prompt);
        status = read_line(buf, size);
        if (status == INPUT_END_OF_FILE) return 0;
        if (status == INPUT_OGILTIGT) printf("Texten är för lång (max %d karaktärer). \n", (int)size -1);
        else if (buf[0] == '\0') printf("Text/Karaktär få inte vara tom. \n");
        else return 1;
    }
}

static InputStatus read_int(const char *prompt, int *out) {
    char buf[LINE_LEN], *end;
    long value;
    InputStatus status;

    printf("%s", prompt);
    if ((status = read_line(buf, sizeof buf)) != INPUT_OK) return status;
    errno = 0;
    value =  strtol(buf, &end, DIGIT_BASE);
    if (end == buf || errno == ERANGE || value > INT_MAX || value < INT_MIN) return INPUT_OGILTIGT;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return INPUT_OGILTIGT;
    *out = (int)value;
    return INPUT_OK;
}

static int read_postition(Registry *reg) {
    int pos;
    InputStatus st;
    for (;;) {
        st = read_int("Position (1-10): ", &pos);
        if (st == INPUT_END_OF_FILE) return -1;
        if (st == INPUT_OK) break;
        printf("EROOR:  Choos another number (1-10). \n");
    }
    if (pos < FIRST_POS || pos > MAX_VEHICLES) {
        printf("ERROR: You need to choose between %d-%d. \n", FIRST_POS, MAX_VEHICLES);
        return -1;
    }
    if (pos - FIRST_POS >= reg->count) {
        printf("EROOR: There is no vehlices in the postition %d. \n", pos);
        return -1;
    }
    return pos - FIRST_POS;
}

static int find_plate(Registry *reg, const char *plate) {
    for (int i = 0; i < reg->count; i++) {
        if (strcmp(reg->vehicles[i].plate, plate) == 0) return 1;
    }
    return 0;
}

/* Shared insert used by both add and random. */
static RegStatus registry_insert(Registry *reg, const Vehicle *v) {
    if (reg->count >= MAX_VEHICLES) return REG_FULL;
    if (find_plate(reg, v->plate)) return REG_DUPLICATE_PLATE;
    reg->vehicles[reg->count] = *v;
    reg->count++;
    return REG_OK;
}

static void report_insert(RegStatus st) {
    if (st == REG_FULL) printf("Fel: registret ar fullt.\n");
    else if (st == REG_DUPLICATE_PLATE) printf("ERROR: Plate nummber already exist. \n");
    else printf("vehicle added.\n");
}

/* MAIN PROGRAM */
static void print_menu(void) {
    printf("\n1. Add Vehicle\n");
    printf("2. Remove Vehicle\n");
    printf("3. Sort\n");
    printf("4. Show Vehicle\n");
    printf("5. Show Registry\n");
    printf("6. Random Vehicle\n");
    printf("0. Quit\n");
}

static void add_vehicle(Registry *reg) {
    Vehicle v;
    int age;

    memset(&v, 0, sizeof v);
    if (!read_text("Fordonstyp: ", v.type, TEXT_LEN)) return;
    if (!read_text("Marke: ", v.brand, TEXT_LEN)) return;
    if (!read_text("Registreringsnummer: ", v.plate, TEXT_LEN)) return;
    if (!read_text("Agarens namn: ", v.owner.name, TEXT_LEN)) return;
    for (;;) {
        InputStatus st = read_int("Agarens alder: ", &age);
        if (st == INPUT_END_OF_FILE) return;
        if (st == INPUT_OK && age >= MIN_AGE && age <= MAX_AGE) break;
        printf("Fel: alder maste vara %d-%d.\n", MIN_AGE, MAX_AGE);
    }
    v.owner.age = age;
    report_insert(registry_insert(reg, &v));
}

static void remove_vehicle(Registry *reg, int position) {
    int idx;
    if (reg->count == 0) {
        printf("Registret ar tomt.\n");
        return;
    }
    idx = read_position(reg);
    if (idx < 0) return;
    for (int i = idx; i < reg->count - 1; i++) {
        reg->vehicles[i] = reg->vehicles[i + 1];
    }
    reg->count--;
    printf("Vehicle removed. \n");
}

static void random_vehicle (Registry *reg, int random) {
    Vehicle v;
    int i;
    memset(&v, 0, sizeof v);
    snprintf(v.type, sizeof v.type, "%s", PICK(RANDOM_TYPES));
    snprintf(v.brand, sizeof v.brand, "%s", PICK(RANDOM_BRANDS));
    snprintf(v.owner.name, sizeof v.owner.name, "%s", PICK(RANDOM_FIRST_NAME, RANDOM_LAST_NAME));
    v.owner.age = RANDOM_MIN_AGE + rand() % (RANDOM_MAX_AGE - RANDOM_MIN_AGE + 1);
    do {
        for (i = 0; i < PLATE_LETTERS; i++) v.plate[i] = PLATE_ALPHABET[(size_t)rand() % (sizeof PLATE_ALPHABET - 1)];
        for (; i < PLATE_LETTERS + PLATE_DIGITS; i++) v.plate[i] = (char)('0' + rand() % DIGIT_BASE);
        v.plate[i] = '\0';
    } while (has_plate(reg, v.plate));
    return v;
}

static void show_vehicle (Registry *reg) {
    int idx;
    if (reg->count == 0) {
        printf("Registret ar tomt.\n");
        return;
    }
    idx = read_position(reg);
    if (idx < 0) return;
    print_vehicle(&reg->vehicles[idx]);
}

static void show_all () {
// Skriver ut hela listan "Endast ett fordon per rad"
}

static int compare_ignore_case (const char *a, const char *b) {
    while (*a && *b && tolower((unsigned char) *a) == tolower((unsigned char) *b)) {
        a++;
        b++;
    }
    return tolower((unsigned char) *a) - tolower((unsigned char) *b);
}

static void sort_vehicle (Registry *reg) {
    int i, j;
    for (i = 1; i < reg->count; i++) {
        Vehicle key = reg->vehicles[i];
        for (j = i - 1; j >= 0 && compare_ignore_case(reg->vehicles[j].brand, key.brand) > 0; j--) {
            reg->vehicles[j + 1] = reg->vehicles[j];
        }
        reg->vehicles[j + 1] = key;
    }
}

static void print_vehicle(Vehicle *v) {
    printf("Fordonstyp: %s\n", v->type);
    printf("Marke:      %s\n", v->brand);
    printf("Reg.nr:     %s\n", v->plate);
    printf("Agare:      %s (%d ar)\n", v->owner.name, v->owner.age);
}

int main(void) {
    Registry reg; 
    int choice, running = 1;
    FileStatus loaded;

    srand((unsigned int)time(NULL));
    loaded = file_load(&reg, REGISTRY_FILE);
    if (loaded == FILE_LOAD_MISSING) {
        printf("No refistry file found. Starting with an empty registry.dat \n");
    }
    else if ( loaded == FILE_LOAD_CORRUPT) {
        printf("Registy file curropted. Starting with an empty registry");
    }

    while (running) {
        InputStatus status;
        print_menu();
        if ((status = read_int("> ", &choice)) == INPUT_END_OF_FILE) {
            break;
        } //input invalid sluta programmet) break;
        if (status == INPUT_OGILTIGT || choice < MENU_QUIT || choice > MENU_RANDOM) {
            printf("ERROR: Choose a number from %d to %d. \n", MENU_QUIT, MENU_RANDOM);
            continue;
        }

        switch (choice) {
            case MENU_ADD: add_vechile(&reg, 0); break;
            case MENU_REMOVE: remove_vehicle(&reg, 1); break;
            case MENU_SORT: sort_vehicle(&reg); printf("Registry data sorted by brand name. \n"); break;
            case MENU_SHOW: show_vehicle(&reg); break;
            case MENU_SHOW_ALL: show_all(&reg); break;
            case MENU_RANDOM: random_vehicle(&reg, 0); break; 
            case MENU_QUIT: running = 0; break; 
        }
    }
}