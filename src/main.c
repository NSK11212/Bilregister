#include <stdio.h>

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

int main() {

    int a;
    scanf("%d", &a);
    printf("%d\n", a);
    return 0;

}