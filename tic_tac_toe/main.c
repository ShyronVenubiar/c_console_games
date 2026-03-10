#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

//The Field
char field[3][3];
//Symbols
char player = 'X';
char enemy = 'O';
//Game-Loop
char repeat = 'j';
int round =  0;

//Set Color
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void instGrid() {
    //Instanciate Grid and assing values to Array
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            field[col][row] = '-';
        }
    }
}

void won(char who) {
    if (who == 'X')
        printf("Du hast gewonnen!\n\n");
    if (who == 'O')
        printf("Der Gegner hat gewonnen!\n\n");
    if (who == 'N')
        printf("Unentschieden!");
    printf("Möchtest du erneut spielen? (j/n)");
    scanf(" %c", &repeat);
    if (repeat == 'j') {
        system("cls");
        instGrid();
        round = 0;
    }
}


int checkWin(char who) {
    for (int i = 0; i < 3; i++) {
        if (field[i][0] == who && field[i][1] == who && field[i][2] == who)
            return 1;
        if (field[0][i] == who && field[1][i] == who && field[2][i] == who)
            return 1;
    }
    if (field[0][0] == who && field[1][1] == who && field[2][2] == who)
        return 1;
    if (field[0][2] == who && field[1][1] == who && field[2][0] == who)
        return 1;
    return 0;
}


int checkIfFree(char cell) {
    if (cell == '-')
        return 1;
    printf("Dieses Feld ist schon belegt!\n");
    return 0;
}


void updateGrid() {
    for (int col = 0; col < 3; col++) {
        for (int row = 0; row < 3; row++) {
            if (field[col][row] == 'X')
                set_color(1);
            if (field[col][row] == 'O')
                set_color(4);
            printf("%c", field[col][row]);
            set_color(7);
        }
        printf("\n");
    }
}

int hasFreeFields() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (field[i][j] == '-') {
                return 1; // There are free Fields
            }
        }
    }
    return 0; // There are NO free Fields
}


void playerTurn() {
    int free = 1;
    do {
        //Controls with T(op), L(eft), R(ight), M(iddle) and D(own) or combination of these
        printf("Wo willst du deinen Stein setzen?\n");
        printf("Drücke eine Zahl, die Anordnung deines NumPads entspicht dem Spielfeld\n -> ");

        //player input, represents numpad order
        int input = 0;
        scanf("%d", &input);

        //check Player input and store it's value
        switch (input) {
            case 7:
                //printf("Oben Links\n");
                if (checkIfFree(field[0][0])) {
                    field[0][0] = player;
                    free = 0;
                }
                break;
            case 8:
                //printf("Oben Mitte\n");
                if (checkIfFree(field[0][1])) {
                    field[0][1] = player;
                    free = 0;
                }
                break;
            case 9:
                //printf("Oben Rechts\n");
                if (checkIfFree(field[0][2])) {
                    field[0][2] = player;
                    free = 0;
                }
                break;
            case 4:
                //printf("Mitte Links\n");
                if (checkIfFree(field[1][0])) {
                    field[1][0] = player;
                    free = 0;
                }
                break;
            case 5:
                //printf("Mitte Mitte\n");
                if (checkIfFree(field[1][1])) {
                    field[1][1] = player;
                    free = 0;
                }
                break;
            case 6:
                //printf("Mitte Rechts\n");
                if (checkIfFree(field[1][2])) {
                    field[1][2] = player;
                    free = 0;
                }
                break;
            case 1:
                //printf("Unten Links\n");
                if (checkIfFree(field[2][0])) {
                    field[2][0] = player;
                    free = 0;
                }
                break;
            case 2:
                //printf("Unten Mitte\n");
                if (checkIfFree(field[2][1])) {
                    field[2][1] = player;
                    free = 0;
                }
                break;
            case 3:
                //printf("Unten Rechts\n");
                if (checkIfFree(field[2][2])) {
                    field[2][2] = player;
                    free = 0;
                }
                break;
            default:
                printf("Dieses Feld gibt es nicht!\n");
        }
    } while (free == 1);

    updateGrid();

    if (checkWin(player))
        won(player);
}


void waitSim(int max) {
    int random = rand() % max + 1;
    for (int i = 0; i < random; i++) {
        printf(".");
        Sleep(200);
    }
    printf("\n\n");
}


void enemyTurn() {
    //Enemy Logic
    //Simulate Thinking
    printf("\nGegner überlegt");
    waitSim(5);
    //First: Try to Win
    printf("Versuche zu gewinnen");
    waitSim(6);
    for (int i = 0; i < 3;i++) {
        for (int j = 0; j < 3; j++) {
            if (field[i][j] == '-') {
                field[i][j] = enemy;
                if (checkWin(enemy)) {
                    updateGrid();
                    won(enemy);
                    return;
                }
                field[i][j] = '-';
            }
        }
    }

    //Second: Try to Block
    printf("Versuche zu blocken");
    waitSim(5);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (field[i][j] == '-') {
                field[i][j] = player;
                if (checkWin(player)) {
                    field[i][j] = enemy;
                    updateGrid();
                    return;
                }
                field[i][j] = '-';
            }
        }
    }

    //Third: Try to place next to own Stone
    printf("Versuche Stein strategisch zu platzieren");
    waitSim(4);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (field[i][j] == enemy) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0) continue;
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < 3 && nj >= 0 && nj < 3 && field[ni][nj] == '-') {
                            field[ni][nj] = enemy;
                            updateGrid();
                            return;
                        }
                    }
                }
            }
        }
    }

    //Forth: Place Random
    printf("Setze Stein zufällig");
    waitSim(2);
    int randcol, randrow;
    do {
        randcol = rand() % 3;
        randrow = rand() % 3;
    } while (field[randcol][randrow] != '-');
    field[randcol][randrow] = enemy;
    updateGrid();
}


int main() {
    system("chcp 1252");
    system("cls");

    srand(time(NULL));
    rand();

    instGrid();

    printf("789\n456\n123\n");

    //Game-Loop
    do {
        // Check if there are free Fields left
        if (!hasFreeFields()) {
            won('N');
            continue;
        }

        if (round > 0) { //Skip first Turn for Enemy, so the Player can begin
            enemyTurn();

            if (checkWin(enemy)) { //If Enemy has NOT won this round, Player's turn
                won(enemy);
                continue;
            }
            // Check if there are free Fields left
            if (!hasFreeFields()) {
                won('N');
                continue;
            }
        }

        //Player's Turn
        playerTurn();

        // Cheeck if Player has won
        if (checkWin(player)) {
            won(player);
            break;
        }

        round++;
    } while (repeat == 'j');

    return 0;
}
