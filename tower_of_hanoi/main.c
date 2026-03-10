#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

#define MIN_FIELDS 3
#define MAX_FIELDS 9

int fields[MAX_FIELDS][MAX_FIELDS]; //Flächen-Array
int heights[MAX_FIELDS];            //Anzahl der Ebenen pro Fläche
int towerHeight;                    //Dynamisch: Höhe = Anzahl der Flächen


//Farbe setzen
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Erstellen der Felder
void drawFields(int fieldCount) {
    int maxHeight = 0;
    for (int i = 0; i < fieldCount; i++) {
        if (heights[i] > maxHeight) maxHeight = heights[i];
    }

    set_color(2); //Farbe auf Grün für die Türme
    //Türme darstellen
    for (int row = maxHeight - 1; row >= 0; row--) {
        for (int f = 0; f < fieldCount; f++) {
            if (row < heights[f]) {
                int size = fields[f][row];
                int width = 2 * size - 1;
                int fieldWidth = 2 * towerHeight - 1 + 2;
                int spaces = (fieldWidth - width) / 2;
                for (int s = 0; s < spaces; s++)
                    printf(" ");
                for (int u = 0; u < width; u++)
                    printf("_");
                for (int s = 0; s < spaces; s++)
                    printf(" ");
            } else {
                int fieldWidth = 2 * towerHeight - 1 + 2;
                for (int s = 0; s < fieldWidth; s++)
                    printf(" ");
            }
            printf("   "); //Abstand der Türme
        }
        printf("\n");
    }

    //Flächen darstellen
    for (int f = 0; f < fieldCount; f++) {
        set_color(3); //Farbe auf Cyan für Flächen und Nummern
        printf("[");                                    //Flächenanfang
        for (int i = 0; i < 2 * towerHeight - 1; i++)
            printf("Y");                                //Flächenfüllung
        printf("]   ");                                 //Flächenende samt Platzhalter
    }
    printf("\n");

    //Flächennummerierung
    for (int f = 0; f < fieldCount; f++) {
        int fieldWidth = 2 * towerHeight + 1; //Fläche berechnen

        for (int s = 0; s < fieldWidth / 2; s++) //Abstand vor der Nummer
            printf(" ");

        printf("%d", f + 1); //Schleife + 1

        for (int s = 0; s < fieldWidth / 2; s++) //Abstand nach der Nummer
            printf(" ");
        printf("   ");
    }
    printf("\n");
    set_color(15); //Zurück zu normaler Farbe
}

//Prüfen ob der Zug gültig ist
int isValidMove(int from, int to) {
    if (heights[from] == 0) // Nichts zum Bewegen
        return 0;
    if (heights[to] == 0) // Ziel leer
        return 1;
    int topFrom = fields[from][heights[from] - 1]; //Ausgangsfeld
    int topTo = fields[to][heights[to] - 1]; //Zielfeld
    return topFrom < topTo; //Ausgangsfeld < Zielfeld
}

//Turmschicht bewegen
void move(int from, int to) {
    int disk = fields[from][--heights[from]]; //Schicht entfernen
    fields[to][heights[to]++] = disk; //Schicht hinzufügen
}

//Prüfen ob das Spiel gewonnen wurde
int gameWon(int fieldCount) {
    if (heights[fieldCount - 1] != towerHeight) //Ist die Höhe erreicht?
        return 0;
    for (int i = 0; i < towerHeight - 1; i++) {
        if (fields[fieldCount - 1][i] <= fields[fieldCount - 1][i + 1])
            return 0;
    }
    return 1;
}

//Wurde das Spiel mit der minimalen Anzahl an Zügen beendet?
int fastSolve(int count, int fields) {
    int minRounds = (int)pow(2, fields) - 1;
    if (count == minRounds)
        return 0;
    return 1;
}


int main() {
    system("chcp 1252");
    system("cls");

    //Variable für die Eingabe des Spielers
    int roundCounter, fieldCount = 3;
    char repeat;

    //Game-Loop
    do{
        //Nutzer nach der Größe fragen
        printf("Wie viele Ringe (3-9)? ");
        scanf("%d", &towerHeight);
        system("cls");



        // Turm auf erstem Feld setzen
        for (int i = towerHeight; i >= 1; i--) {
            fields[0][towerHeight - i] = i;
        }
        heights[0] = towerHeight;

        for (int i = 1; i < fieldCount; i++) {
            heights[i] = 0;
        }

        //InGame-Loop
        while (!gameWon(fieldCount)) { //Wiederholen bis Spiel gewonnen wurde
            drawFields(fieldCount); //Felder erstellen

            int from, to;

            //Ausgangspunkt vom Spieler festlegen lassen
            printf("\nVon welcher Fläche nehmen: ");
            from = getch() - '0';
            //Und auf Gültigkeit prüfen
            if (from < 1 || from > fieldCount) {
                printf("Ungültige Eingabe!\n");
                continue;
            }

            //Zielpunkt vom Spieler festlegen lassen
            printf("%d\nWohin legen: ", from);
            to = getch() - '0';
            //Und auf Gültigkeit prüfen
            if (to < 1 || to > fieldCount) {
                printf("Ungültige Eingabe!\n");
                continue;
            }

            roundCounter++;

            printf("%d\n\n", to);

            //Ist der Zug gültig?
            if (isValidMove(from - 1, to - 1)) { //Eins abziehen, weil Zahl != Arrayzahl
                move(from - 1, to - 1);
            } else {
                printf("Ungültiger Zug!\n");
            }
        }

        drawFields(fieldCount);
        if (fastSolve(roundCounter, towerHeight)) {
            printf("Glückwunsch! Du hast den Turm erfolgreich verschoben!\n");
        }
        else{
            printf("Glückwunsch! Du hast den Turm auf die schnellste Art und Weise verschoben!\n");
        }

        do {
            printf("\n\nMöchtest du erneut spielen? (j/n)");
            fflush(stdin);
            scanf(" %c", &repeat);
            system("cls");
        } while (repeat != 'j' && repeat != 'n');
    } while (repeat == 'j');

    return 0;
}
