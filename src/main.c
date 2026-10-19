#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/* 
Skapa en set av instruktioner:
Lägg till fordon
 -> Fordonets reg nummer
 -> Fordonets typ
 -> Märke
 -> Ägaren namn och ålder
Programet ska kunna avlsutas och sedan spara register.txt filen.
Om filen inte finns ska den skappa.
Om programet startas igen ska den kunna läsa in register.txt (MAX 10 fordon)
för att se om listan är full eller om det finns plats för fler fordon
och läsa in alla fordon som finns i register.txt filen om användaren vill söka.

Programet startar på en meny.
Om Menyval show ska det så kommer andvändaren kunna se all fordon som finns i register.txt
sedan välja från 1-10 vilket av fordonen som ska vissas detaljer om. (ägare, reg num).
*/

/* 
Sotering av fordonen i register.txt i bokstavsordning efter märke.
Har en funcktion som kan skriva ut hela registeret i bokstavsordning efter märke.
slumpa fordon. Ett slumpad fordon ska skapas, med rimliga värden. För varje egenskap hos ett
fordon ska det finnas minst 10 möjliga slumvärden.
*/

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

static void quit_program() {
    
}

int main(void) {
    int choice;

    switch (choice) {
        case MENU_ADD: add_vechile; break;
        case MENU_QUIT: quit_program; break; 
    }
}
