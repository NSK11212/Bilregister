#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <file.h>

#define MIN_AGE 18
#define MAX_AGE 100
#define RANDOM_MIN_AGE 18
#define RANDOM_MAX_AGE 90
#define PLATE_LETTERS 3
#define PLATE_DIGITS 3
#define DIGIT_BASE 10
#define LINE_LEN 64
#define COL_GAP 2

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
static const char *const RANDOM_FIRST_NAME = {"Anna", "Adam", "Ellinor", "Elliot", "Christina", "Christer", "Lars", "Lena", "Maria", "Marcus", "Roger", "Rebecca", "Sara", "Simon"};
static const char *const RANDOM_LAST_NAME = {"Andersson", "Lindberg", "Lundberg", "Holm", "Olsson", "Ali", "Lindqvist", "Sandberg", "Forsberg", "Berglund", "Eklund"};

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
        print("%s", prompt);
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

// File logic

static RegStatus registry_add(Registry *reg, ) {

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

static void add_vehicle(Registry *reg, int random) {
    Vehicle b;
    RegStatus status;   
}

static void remove_vehicle(Registry *reg, int random) {
// Från 1-10 position "Tänk på att vektorer börjar på 0"
}

static void random_vehicle (Registry *reg, int random) {
// Slumpgenration För Funktioner 6 i labb listan
}

static void show_vehicle () {

}

static void show_all () {
// Skriver ut hela listan "Endast ett fordon per rad"
}

static void sort_vehicle () {
// Soteras i BOKSTAVS ordining efter BILMÄRKE
}

static void print_vehicle () {
// Från 1-10 position "Tänk på att vektorer börjar på 0"
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