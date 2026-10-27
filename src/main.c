#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file.h"

#define MIN_AGE 18
#define MAX_AGE 100
#define LINE_LEN 64
#define COL_GAP 2
#define FIRST_POS 1
#define TABLE_COLS 6
#define PICK(arr) ((arr)[(size_t)rand() % (sizeof (arr) / sizeof (arr)[0])])

typedef enum {
    QUIT,
    add,
    taBort,
    sortera,
    visa,
    visaAll,
    RANDOM,
    LAST = RANDOM
} MenuChoice;

typedef enum {
    Reg_OK,
    Regnum_FULL,
    Regnum_DUPLICATE
} RegStatus;

static const char *const BRANDS[] = {"Audi", "BMW", "Ford", "Honda", "Kia", "Peugeot", "Renault", "Saab", "Tesla", "Toyota", "Volkswagen", "Volvo"};
static const char *const TYPES[] = {"SUV", "Sedan", "Coupe", "Van", "Minivan", "Pickup", "Convertible", "Crossover"};
static const char *const FIRST_NAMES[] = {"Anna", "Erik", "Maria", "Johan", "Karin", "Lars"};
static const char *const LAST_NAMES[] = {"Andersson", "Johansson", "Karlsson", "Nilsson", "Eriksson", "Larsson"};

static const char PLATE_ALPHABET[] = "ABCDEFGHJKLMNOPRSTUWXYZ";

int read_num(const char *prompt, int *out) {
    char buf[LINE_LEN];
 
    printf("%s", prompt);

    if (fgets(buf, LINE_LEN, stdin) == NULL) {
        return -1;
    }
    if (sscanf(buf, "%d", out) != 1) {
        return 0;
    }

    return 1;
}

int read_text(const char *prompt, char *out, int size) {
    char buf[LINE_LEN];
 
    printf("%s", prompt);
    if (fgets(buf, LINE_LEN, stdin) == NULL) {
        return 0;
    }

    buf[strcspn(buf, "\n")] = '\0';
    
    if (strlen(buf) == 0) {
        return 0;
    }

    strncpy(out, buf, size - 1);
    out[size - 1] = '\0';
    return 1;
}

int position(const Registryfil *reg) {
    int pos;
 
    if (read_num("Position: ", &pos) != 1) {
        printf("Invalid position.\n");
        return -1;
    }
    if (pos < 1 || pos > reg->count) {
        printf("Invalid position or no vehicle there.\n");
        return -1;
    }

    return pos - 1;
}

int plateExist(const Registryfil *reg, const char *plate) {
    for (int i = 0; i < reg->count; i++) {
        if (strcmp(reg->vehicles[i].plate, plate) == 0) return 1;
    }
    return 0;
}

static RegStatus registryInsert(Registryfil *reg, const Vehicle *v) {
    if (reg->count >= MAX_VEHICLES) {
        return Regnum_FULL;
    }

    reg->vehicles[reg->count] = *v;
    reg->count++;

    return Reg_OK;
}

static void printInfo(const Vehicle *v) {
    printf("Type:   %s\n", v->type);
    printf("Brand:  %s\n", v->brand);
    printf("Plate:  %s\n", v->plate);
    printf("Owner:  %s (%d years)\n", v->owner.name, v->owner.age);
}

static void report(RegStatus st, const Vehicle *v) {
    if (st == Regnum_FULL) {
        printf("ERROR: The registry is full.\n");
    }
    else if (st == Regnum_DUPLICATE) {
        printf("ERROR: Plate number already exists.\n");
    }
    else {
        printf("Vehicle added. \n");
        printInfo(v);
    }
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

static void printRow(const char *const cells[TABLE_COLS], const int widths[TABLE_COLS]) {
    for (int c = 0; c < TABLE_COLS; c++) {
        printf("%-*s%*s", widths[c], cells[c], COL_GAP, "");
    }

    printf("\n");
}

static void showAllVehicle(const Registryfil *reg) {
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

    printRow(headers, widths);

    for (int i = 0; i < reg->count; i++) {
        const Vehicle *v = &reg->vehicles[i];
        const char *cells[TABLE_COLS] = {pos_buf[i], v->type, v->brand, v->plate, v->owner.name, age_buf[i]};
        printRow(cells, widths);
    }
}

static void showVehicle(const Registryfil *reg) {
    int idx;

    if (reg->count == 0) {
        printf("The registry is empty.\n");
        return;
    }

    idx = position(reg);

    if (idx < 0) {
        return;
    }

    printInfo(&reg->vehicles[idx]);
}

static void addVehicle(Registryfil *reg) {
    Vehicle v;
    int age;

    if (reg->count >= MAX_VEHICLES) {
        report(Regnum_FULL, NULL);
        return;
    }

    memset(&v, 0, sizeof v);

    if (!read_text("Vehicle type: ", v.type, TEXT_LEN)) {
        return;
    }
    if (!read_text("Brand: ", v.brand, TEXT_LEN)) {
        return;
    }
    if (!read_text("Plate number: ", v.plate, TEXT_LEN)) {
        return;
    }
    if (!read_text("Owner name: ", v.owner.name, TEXT_LEN)) {
        return;
    }

    for (;;) {
        int st = read_num("Owner age: ", &age);
        if (st == -1) return;
        if (st == 1 && age >= MIN_AGE && age <= MAX_AGE) break;
        printf("ERROR: Age must be between %d and %d.\n", MIN_AGE, MAX_AGE);
    }

    v.owner.age = age;
    report(registryInsert(reg, &v), &v);
}

static void removeVehicle(Registryfil *reg) {
    int idx;

    if (reg->count == 0) {
        printf("The registry is empty.\n");
        return;
    }

    idx = position(reg);

    if (idx < 0) {
        return;
    }

    for (int i = idx; i < reg->count - 1; i++) {
        reg->vehicles[i] = reg->vehicles[i + 1];
    }

    reg->count--;
    memset(&reg->vehicles[reg->count], 0, sizeof reg->vehicles[reg->count]);
    printf("Vehicle removed.\n");
}

static void randomVehicle(Registryfil *reg) {
    Vehicle v;

    if (reg->count >= MAX_VEHICLES) {
        report(Regnum_FULL, NULL);
        return;
    }

    memset(&v, 0, sizeof v);

    strcpy(v.type, PICK(TYPES));
    strcpy(v.brand, PICK(BRANDS));
    sprintf(v.owner.name, "%s %s", PICK(FIRST_NAMES), PICK(LAST_NAMES));
    v.owner.age = MIN_AGE + rand() % (MAX_AGE - MIN_AGE + 1);

    do {
        sprintf(v.plate, "%c%c%c%03d", 
            PLATE_ALPHABET[rand() % (sizeof(PLATE_ALPHABET) - 1)],
            PLATE_ALPHABET[rand() % (sizeof(PLATE_ALPHABET) - 1)],
            PLATE_ALPHABET[rand() % (sizeof(PLATE_ALPHABET) - 1)],
            rand() % 1000);
    } while (plateExist(reg, v.plate));

    report(registryInsert(reg, &v), &v);
}

static void sortVehicle(Registryfil *reg) {
    int i, j;
    Vehicle temp;

    for (i = 0; i < reg->count - 1; i++) {
        for (j = 0; j < reg->count - i - 1; j++) {
            if (strcasecmp(reg->vehicles[j].brand, reg->vehicles[j + 1].brand) > 0) {
                temp = reg->vehicles[j];
                reg->vehicles[j] = reg->vehicles[j + 1];
                reg->vehicles[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    Registryfil reg;
    int choice = 0, running = 1;

    memset(&reg, 0, sizeof reg);

    if (file_load(&reg, REGISTRY_FILE) != file_load_OK) {
        printf("Could not load registry, starting with empty registry.\n");
    }

    srand((unsigned int)time(NULL));

    while (running) {
        int status;

        print_menu();
        status = read_num("> ", &choice);

        if (status == -1) {
            break;
        }
        if (status != 1 || choice < QUIT || choice > LAST) {
            printf("ERROR: Choose a number from %d to %d.\n", QUIT, LAST);
            continue;
        }

        switch (choice) {
            case add:
                addVehicle(&reg);
                break;
            case taBort:
                removeVehicle(&reg);
                break;
            case sortera:
                sortVehicle(&reg);
                printf("Registry sorted by brand. \n");
                showAllVehicle(&reg); break;
            case visa:
                showVehicle(&reg);
                break;
            case visaAll:
                showAllVehicle(&reg);
                break;
            case RANDOM:
                randomVehicle(&reg);
                break;
            case QUIT:
                running = 0;
                break;
        }
    }

    if (file_save(&reg, REGISTRY_FILE) != file_SAVE) {
        printf("ERROR: Could not save the registry to %s.\n", REGISTRY_FILE);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}