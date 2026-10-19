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
} Regstats;

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

static void add_vehicle() {
/* Lägg till bill
fråga efter namn, 
            ålder, 
            bilmärke, 
            fordontyp, 
            regnummer.
*/    
}

static void remove_vehicle() {
// Från 1-10 position "Tänk på att vektorer börjar på 0"
}

static void random_vehicle () {
// Slumpgenration För Funktioner 6 i labb listan
}

static void show_vehicle () {

}

static void show_registry_vehicle () {
// Skriver ut hela listan "Endast ett fordon per rad"
}

static void sort_vehicle () {
// Soteras i BOKSTAVS ordining efter BILMÄRKE
}

static void print_vehicle () {
// Från 1-10 position "Tänk på att vektorer börjar på 0"
}

int main(void) {
    int choice;
    int running = 1;

    while (running) {
        print_menu();
        if (//input invalid sluta programmet) break;
        switch (choice) {
            case MENU_ADD: add_vechile(); break;
            case MENU_QUIT: running = 0; break; 
        }
    }
}
