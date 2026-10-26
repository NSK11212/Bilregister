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
#define DECIMAL_BASE 10
#define LINE_LEN 64
#define COL_GAP 2
#define FIRST_POS 1
#define TABLE_COLS 6

#define PICK(arr) ((arr)[(size_t)rand() % (sizeof (arr) / sizeof (arr)[0])])

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
    REG_DUPLICATE_PLATE
} RegStatus;

typedef enum {
    INPUT_OK,
    INPUT_INVALID,
    INPUT_END_OF_FILE
} InputStatus;

static const char *const RANDOM_BRANDS[] = {"Audi", "BMW", "Ford", "Honda", "Kia", "Peugeot", "Renault", "Saab", "Tesla", "Toyota", "Volkswagen", "Volvo"};
static const char *const RANDOM_TYPES[] = {"SUV", "Sedan", "Coupe", "Van", "Minivan", "Pickup", "Convertible", "Crossover"};
static const char *const RANDOM_FIRST_NAMES[] = {"Anna", "Erik", "Maria", "Johan", "Karin", "Lars"};
static const char *const RANDOM_LAST_NAMES[] = {"Andersson", "Johansson", "Karlsson", "Nilsson", "Eriksson", "Larsson"};

static const char PLATE_ALPHABET[] = "ABCDEFGHJKLMNOPRSTUWXYZ";

static InputStatus read_line(char *buf, size_t size) {
    size_t len;
    int c;

    if (fgets(buf, (int)size, stdin) == NULL) return INPUT_END_OF_FILE;
    len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') { buf[len - 1] = '\0'; return INPUT_OK; }
    if (feof(stdin)) return INPUT_OK;

    c = getchar();
    if (c == '\n' || c == EOF) return INPUT_OK;
    while (c != '\n' && c != EOF) c = getchar();
    return INPUT_INVALID;
}

static int read_text(const char *prompt, char *buf, size_t size) {
    for (;;) {
        InputStatus status;
        printf("%s", prompt);
        status = read_line(buf, size);
        if (status == INPUT_END_OF_FILE) return 0;
        if (status == INPUT_INVALID) printf("Text is too long (max %d characters).\n", (int)size - 1);
        else if (buf[0] == '\0') printf("Text must not be empty.\n");
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
    value = strtol(buf, &end, DECIMAL_BASE);
    if (end == buf || errno == ERANGE || value > INT_MAX || value < INT_MIN) return INPUT_INVALID;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return INPUT_INVALID;
    *out = (int)value;
    return INPUT_OK;
}

static int read_position(const Registry *reg) {
    int pos;
    InputStatus st;

    for (;;) {
        st = read_int("Position: ", &pos);
        if (st == INPUT_END_OF_FILE) return -1;
        if (st == INPUT_OK) break;
        printf("ERROR: Enter a number.\n");
    }
    if (pos < FIRST_POS || pos > MAX_VEHICLES) {
        printf("ERROR: Position must be between %d and %d.\n", FIRST_POS, MAX_VEHICLES);
        return -1;
    }
    if (pos - FIRST_POS >= reg->count) {
        printf("ERROR: There is no vehicle at position %d.\n", pos);
        return -1;
    }
    return pos - FIRST_POS;
}

static int has_plate(const Registry *reg, const char *plate) {
    for (int i = 0; i < reg->count; i++) {
        if (strcmp(reg->vehicles[i].plate, plate) == 0) return 1;
    }
    return 0;
}

static RegStatus registry_insert(Registry *reg, const Vehicle *v) {
    if (reg->count >= MAX_VEHICLES) {
        return REG_FULL;
    }
    if (has_plate(reg, v->plate)) {
        return REG_DUPLICATE_PLATE;
    }
    reg->vehicles[reg->count] = *v;
    reg->count++;
    return REG_OK;
}

static void print_vehicle(const Vehicle *v) {
    printf("Type:   %s\n", v->type);
    printf("Brand:  %s\n", v->brand);
    printf("Plate:  %s\n", v->plate);
    printf("Owner:  %s (%d years)\n", v->owner.name, v->owner.age);
}

static void report_insert(RegStatus st, const Vehicle *v) {
    if (st == REG_FULL) {
        printf("ERROR: The registry is full.\n");
    }
    else if (st == REG_DUPLICATE_PLATE) {
        printf("ERROR: Plate number already exists.\n");
    }
    else {
        printf("Vehicle added. \n");
        print_vehicle(v);
    }
}

static int compare_ignore_case(const char *a, const char *b) {
    while (*a && *b && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

static void print_menu(void) {
    printf("\n1. Add Vehicle\n");
    printf("2. Remove Vehicle\n");
    printf("3. Sort\n");
    printf("4. Show Vehicle\n");
    printf("5. Show Registry\n");
    printf("6. Random Vehicle\n");
    printf("0. Quit\n");
}

static void print_row(const char *const cells[TABLE_COLS], const int widths[TABLE_COLS]) {
    for (int c = 0; c < TABLE_COLS; c++) {
        printf("%-*s%*s", widths[c], cells[c], COL_GAP, "");
    }
    printf("\n");
}

static void show_all(const Registry *reg) {
    static const char *const headers[TABLE_COLS] = {"Pos", "Type", "Brand", "Plate", "Owner", "Age"};
    char pos_buf[MAX_VEHICLES][16];
    char age_buf[MAX_VEHICLES][16];
    int widths[TABLE_COLS];

    if (reg->count == 0) {
        printf("The registry is empty.\n");
        return;
    }

    for (int c = 0; c < TABLE_COLS; c++) widths[c] = (int)strlen(headers[c]);

    for (int i = 0; i < reg->count; i++) {
        const Vehicle *v = &reg->vehicles[i];
        const char *cells[TABLE_COLS];

        snprintf(pos_buf[i], sizeof pos_buf[i], "%d", i + FIRST_POS);
        snprintf(age_buf[i], sizeof age_buf[i], "%d", v->owner.age);
        cells[0] = pos_buf[i];
        cells[1] = v->type;
        cells[2] = v->brand;
        cells[3] = v->plate;
        cells[4] = v->owner.name;
        cells[5] = age_buf[i];
        for (int c = 0; c < TABLE_COLS; c++) {
            int len = (int)strlen(cells[c]);
            if (len > widths[c]) widths[c] = len;
        }
    }

    print_row(headers, widths);
    for (int i = 0; i < reg->count; i++) {
        const Vehicle *v = &reg->vehicles[i];
        const char *cells[TABLE_COLS] = {pos_buf[i], v->type, v->brand, v->plate, v->owner.name, age_buf[i]};
        print_row(cells, widths);
    }
}

static void add_vehicle(Registry *reg) {
    Vehicle v;
    int age;

    if (reg->count >= MAX_VEHICLES) {
        report_insert(REG_FULL, NULL);
        return;
    }

    memset(&v, 0, sizeof v);
    if (!read_text("Vehicle type: ", v.type, TEXT_LEN)) return;
    if (!read_text("Brand: ", v.brand, TEXT_LEN)) return;
    if (!read_text("Plate number: ", v.plate, TEXT_LEN)) return;
    if (!read_text("Owner name: ", v.owner.name, TEXT_LEN)) return;
    for (;;) {
        InputStatus st = read_int("Owner age: ", &age);
        if (st == INPUT_END_OF_FILE) return;
        if (st == INPUT_OK && age >= MIN_AGE && age <= MAX_AGE) break;
        printf("ERROR: Age must be between %d and %d.\n", MIN_AGE, MAX_AGE);
    }
    v.owner.age = age;
    report_insert(registry_insert(reg, &v), &v);
}

static void remove_vehicle(Registry *reg) {
    int idx;

    if (reg->count == 0) {
        printf("The registry is empty.\n");
        return;
    }
    idx = read_position(reg);
    if (idx < 0) return;
    for (int i = idx; i < reg->count - 1; i++) {
        reg->vehicles[i] = reg->vehicles[i + 1];
    }
    reg->count--;
    memset(&reg->vehicles[reg->count], 0, sizeof reg->vehicles[reg->count]);
    printf("Vehicle removed.\n");
}

static void random_vehicle(Registry *reg) {
    Vehicle v;
    int i;

    if (reg->count >= MAX_VEHICLES) {
        report_insert(REG_FULL, NULL);
        return;
    }

    memset(&v, 0, sizeof v);
    snprintf(v.type, sizeof v.type, "%s", PICK(RANDOM_TYPES));
    snprintf(v.brand, sizeof v.brand, "%s", PICK(RANDOM_BRANDS));
    snprintf(v.owner.name, sizeof v.owner.name, "%s %s", PICK(RANDOM_FIRST_NAMES), PICK(RANDOM_LAST_NAMES));
    v.owner.age = RANDOM_MIN_AGE + rand() % (RANDOM_MAX_AGE - RANDOM_MIN_AGE + 1);
    do {
        for (i = 0; i < PLATE_LETTERS; i++) {
            v.plate[i] = PLATE_ALPHABET[(size_t)rand() % (sizeof PLATE_ALPHABET - 1)];
        }
        for (; i < PLATE_LETTERS + PLATE_DIGITS; i++) {
            v.plate[i] = (char)('0' + rand() % DECIMAL_BASE);
        }
        v.plate[i] = '\0';
    } while (has_plate(reg, v.plate));

    report_insert(registry_insert(reg, &v), &v);
}

static void show_vehicle(const Registry *reg) {
    int idx;

    if (reg->count == 0) {
        printf("The registry is empty.\n");
        return;
    }
    idx = read_position(reg);
    if (idx < 0) return;
    print_vehicle(&reg->vehicles[idx]);
}

static void sort_vehicle(Registry *reg) {
    int i, j;
    for (i = 1; i < reg->count; i++) {
        Vehicle key = reg->vehicles[i];
        for (j = i - 1; j >= 0 && compare_ignore_case(reg->vehicles[j].brand, key.brand) > 0; j--) {
            reg->vehicles[j + 1] = reg->vehicles[j];
        }
        reg->vehicles[j + 1] = key;
    }
}

int main(void) {
    Registry reg;
    int choice = 0, running = 1;
    FileStatus status_file;

    memset(&reg, 0, sizeof reg);
    srand((unsigned int)time(NULL));

    status_file = file_load(&reg, REGISTRY_FILE);
    if (status_file == FILE_LOAD_MISSING) {
        printf("No registry file found. Starting with an empty registry.\n");
    } else if (status_file == FILE_LOAD_CORRUPT) {
        printf("Registry file is corrupted. Starting with an empty registry.\n");
    }

    while (running) {
        InputStatus status;

        print_menu();
        status = read_int("> ", &choice);
        if (status == INPUT_END_OF_FILE) break;
        if (status == INPUT_INVALID || choice < MENU_QUIT || choice > MENU_LAST) {
            printf("ERROR: Choose a number from %d to %d.\n", MENU_QUIT, MENU_LAST);
            continue;
        }

        switch (choice) {
            case MENU_ADD:      add_vehicle(&reg); break;
            case MENU_REMOVE:   remove_vehicle(&reg); break;
            case MENU_SORT:     sort_vehicle(&reg); printf("Registry sorted by brand. \n"); show_all(&reg); break;
            case MENU_SHOW:     show_vehicle(&reg); break;
            case MENU_SHOW_ALL: show_all(&reg); break;
            case MENU_RANDOM:   random_vehicle(&reg); break;
            case MENU_QUIT:     running = 0; break;
        }
    }

    if (file_save(&reg, REGISTRY_FILE) != FILE_SAVE_OK) {
        printf("ERROR: Could not save the registry to %s.\n", REGISTRY_FILE);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}