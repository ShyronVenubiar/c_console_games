#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>


//Values for the Board
int boardSize = 8;
char (*frontField)[4]; //Actual visible Field
char (*backField)[4]; //Recover Field that only stores the original values to come back to them later
int tiles;
int count = 0;

//Player Dice
int dice;
bool diceRolled = false;
int wrongCount = 0;

//Set Player
char player = '@';
int playerPos = 0;
int newPos;

//Variables for Objects
int (*snakes)[2];
int (*ladders)[2];
int amountS;
int amountL;
bool snakebite = false;
bool ladderclimb = false;
int highestSnake = 0;
bool extremeSnake = false;
int minSnakes;
int maxSnakes;


//For difficulty differences
char difficulty;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void titleScreen() {
    //Titlescreen
    printf("*************************************************\n");
    printf("*                                               *\n");
    printf("*               SNAKES AND LADDERS              *\n");
    printf("*                                               *\n");
    printf("*                    BY FELIX                   *\n");
    printf("*                                               *\n");
    printf("*************************************************\n\n\n");
    printf("Press SPACEBAR to Start the Game...");

    //Press Start to play AND ONLY START!!
    char startGame;
    do {
    startGame = getch();
    } while (startGame != ' ');

    Sleep(0.5);

    //Clear Screen
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void setBoardSize() {
    //Let Player set Board Size
    printf("Welche Spielfeldgroesse bevorzugst du?\n");
    printf("7, 8, 9\n");
    int choosenSize = getch() - '0';
    if (choosenSize == 7 || choosenSize == 8 || choosenSize == 9) {
        boardSize = choosenSize;
    }
    else {
        setColor(4);
        printf("Bitte gib eine gueltige Zahl ein\n");
        setColor(7);
        setBoardSize();
    }
}

void setDifficulty() {
    printf("\nWaehle deine Schwierigkeit\n");
    printf("(N)ormal:\nDas Spiel wie man es als Brettspiel aus der Kindheit kennt.\n");
    printf("(S)chwer:\nWeniger Leiter, mehr Schlangen und du musst exakt auf das letzte Feld kommen um zu gewinnen\n");
    printf("(E)xtrem:\nKeine Leitern, mindestens 1 Schlange nahe des letzten Feldes, D4 statt D6\n");
    while(1) {
        scanf(" %c", &difficulty);
        if (difficulty == 'n' || difficulty == 's' || difficulty == 'e') {
            break;
        } else {
            printf("Bitte waehle eine gueltige Schwierigkeit\n");
        }
    }
}

void buildBoard() {
    //Build Board
    for (int j = 0; j < boardSize; j++) {
        for (int i = 1; i < boardSize + 1; i++) {
            sprintf(frontField[count], "%02d", count + 1);  //Save Value to Visible Array as formatted String
            count++;
        }
    }
}

void printBoard() {
    //Create Space
    printf("\n\n");
    //Create Print
    int count = boardSize * boardSize - 1;                  //set count to be the last position in Array
    for (int i = 0; i < boardSize; i++) {
        if (i % 2 == 0) { //Odd Row
            for (int j = 0; j < boardSize; j++) {
                if (frontField[count][0] == 'S') {          //If field is a Snake
                    setColor(2);                            //print in green
                } else if (frontField[count][1] == '@') {   //If field is a Player
                    setColor(4);                            //print in red
                } else if (frontField[count][0] == 'L') {   //If field is a Ladder
                    setColor(3);                            //print in Blue
                } else {
                    setColor(7);                            //print white for every other field
                }
                printf("%s ", frontField[count]);
                count--; //count backwards for odd Row
            }
        } else { //Even Row
            count -= boardSize - 1;
            for (int j = 0; j < boardSize; j++) {
                if (frontField[count][0] == 'S') {          //If field is a Snake
                    setColor(2);                            //print in green
                } else if (frontField[count][1] == '@') {   //If field is a Player
                    setColor(4);                            //print in red
                } else if (frontField[count][0] == 'L') {   //If field is a Ladder
                    setColor(3);                            //print in Blue
                } else {
                    setColor(7);                            //print white for every other field
                }
                printf("%s ", frontField[count]);
                count++;                                    //Count forwards for even Row
            }
            count -= boardSize + 1;                         //Readjust counter position for next column
        }
        printf("\n");
    }
    setColor(7);                                            //set color to white in case it changed
}

void closeApp() {
    free(frontField);
    free(backField);
    free(snakes);
    free(ladders);
    exit(0);
}

void rollDice() {
    //Interaction with the Player
    char input;
    //Wait a Short time
    Sleep(300);
    printf("\nPress Spacebar to roll the Dice\n\n");

    //Roll the Dice
    while(1) {
        input = getch();

        if (input == ' ') {
            diceRolled = true;
            if (difficulty == 'e') {
                dice = rand() % 4 + 1;                              //Random Number between 1 - 4 for Extreme difficulty
            } else {
                dice = rand() % 6 + 1;                              //Random Number between 1 - 6
            }

            //Reset Easteregg if activated
            if (wrongCount >= 5) {
                wrongCount = 4;
            }
            Sleep(500);
            break;
        } else if (input == 'x') {
            closeApp();
        } else {
            wrongCount++;
            if (wrongCount >= 5) {
                printf("Press SPACEBAR!!!!! to roll the Dice\n\n");
            } else {
                printf("Press Spacebar to roll the Dice\n\n");
            }
        }
    }

    printf("Du hast eine %i gewuerfelt!\n\n", dice);
    Sleep(800);
}

void movePlayer() {
    //Restore old Field
    frontField[playerPos][0] = backField[playerPos][0];     //First Digit
    frontField[playerPos][1] = backField[playerPos][1];     //Second Digit

//    printf("Spielfeldposition wurde wiederhergestellt\n");

    //Movement because of dice throw?
    if (diceRolled == true) {
        //Check if Player would exceed the upper limit
        if (playerPos + dice > tiles - 1) {
            if (difficulty == 'e' || difficulty == 's') {
                printf("Du hast zu viel gewuerfelt!\n");
                //Player won't be moved
                printf("Current Position: %i, Position after Dice: %i", playerPos, playerPos + dice);
            }
//            //If so, Player position is last Field
//            playerPos = tiles - 1;
        }
        else {
            playerPos += dice;                              //Move Player to new position
        }
//        printf("Neue Spielerposition nach dem Würfeln ist %i\n", playerPos + 1);
        diceRolled = false;
    }

    //Movement because of Snakebite?
    if (snakebite == true) {
        //Inform Player that he hit a snakes head
        printf("\n\nDu bist einer Schlange auf den Kopf getreten!\nSie hat dich gefressen...");
        Sleep(500);
        playerPos = newPos;
//        printf("Neue Spielerposition nach Schlangenbiss sollte %i sein und ist nun %i\n", newPos, playerPos);
        snakebite = false;
    }

    //Movement because of Ladder?
    if (ladderclimb == true) {
        //Inform Player that he found a ladder to climb
        printf("\n\nDu hast eine Leiter gefunden und kletterst sie rauf");
        Sleep(500);
        playerPos = newPos;
//        printf("Neue Spielerposition nach dem erzwingen der Leiter sollte %i sein und ist nun %i\n", newPos, playerPos);
        ladderclimb = false;
    }

    //Set new Player position on the Field
    frontField[playerPos][0] = ' ';                         //First Digit
    frontField[playerPos][1] = '@';                         //Second Digit

    //Print updated Board
    printBoard();

//    printf("Spielerposition wurde auf Spielfeld gesetzt\n");
}

void wonGame() {
    //WON!!
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("*************************************************\n");
    printf("*                                               *\n");
    printf("*                  GLUECKWUNSCH                 *\n");
    printf("*                                               *\n");
    printf("*                DU HAST GEWONNEN!              *\n");
    printf("*                                               *\n");
    printf("*************************************************\n\n\n");

    printf("Moechtest du nochmal spielen?\n(j)a | (n)ein\n");
    char input;
    scanf(" %c", &input);
    switch (input){
        case 'j':
            system("\"Snakes and Ladders.exe\"");
            closeApp();
            break;
        case 'n':
            closeApp();
            break;
        default:
            printf("Bitte gib (j)a oder (n)ein ein\n");
    }
}

void checkPos() {
    //Check if Player stands on a Snake
    if (backField[playerPos][0] == 'S') { //Check Backupfield if there is a S on this position, if so, check which one
//        printf("Spieler ist auf eine Schlange getreten\n");
        //Check Snake number on this position
        for (int j = 0; j < amountS - 1; j++) {
            //Check if Player stands on Head or Tail
            if (snakes[j][0] == playerPos) {
//                printf("Es war der Kopf\n");
                //Player stands on the Snakes head
                newPos = snakes[j][1]; //Set the new Player Position to the Snakes Tail
//                printf("Der Schwanz befindet sich auf Feld %i\n", newPos);
                snakebite = true;
                movePlayer();
                break;
            }
            else if (snakes[j][1] == playerPos) {
//                printf("Es war nur der Schwanz\n");
                break;
                //Player stands on the Snakes Tail => nothing happens
            }
        }
    }

    //Check if Player stands on a Ladder
    if (backField[playerPos][0] == 'L') { //Check Backupfield if there is a L on this position, if so, check which one
//        printf("Spieler ist auf eine Leiter getreten\n");
        //Check Ladder number on this position
        for (int h = 0; h < amountL - 1; h++) {
            //Check if Player stands on Top or Bottom
            if (ladders[h][0] == playerPos) {
//                printf("Höher gehts nicht\n");
                break;
                //Player already reached Top of this Ladder => nothing happens
            } else if (ladders[h][1] == playerPos) {
//                printf("Spieler steht unten an der Leiter\n");
                //Player stands on the bottom of the ladder
                newPos = ladders[h][0]; //Set the new Player position to the top of the ladder
//                printf("Die Spitze der Leiter befindet sich auf %i\n", newPos);
                ladderclimb = true;
                movePlayer();
                break;
            }
        }
    }

    //Check if Player reached the last Tile
    if (playerPos >= tiles - 1) { //WON!!
        wonGame();
    }
}

void setObjectives() {
    //SNAKES
    //Calculate Snake quantity
    if (difficulty == 'e') {
        minSnakes = tiles / 13;
        maxSnakes = tiles / 8;
        amountS = rand() % (maxSnakes - minSnakes + 1) + minSnakes; //MUCH more Snakes
        extremeSnake = true;
    } else if (difficulty == 's') {
        minSnakes = tiles / 17;
        maxSnakes = tiles / 13;
        amountS = rand() % (maxSnakes - minSnakes + 1) + minSnakes; //More Snakes
    } else {
        minSnakes = tiles / 32;
        maxSnakes = tiles / 21;
        amountS = rand() % (maxSnakes - minSnakes + 1) + minSnakes; //Normal Amount of Snakes
    }

    //Create Multidimensional Snake Array for Start and End of the Snakes
    snakes = calloc(amountS, sizeof(int) * 2);
    if (snakes == NULL) {
        printf("Fehler: Speicher konnte nicht allokiert werden!\n");
//        exit(1);
    }
    //Position Snakes
    for (int i = 0; i < amountS; i++) {
        if (extremeSnake == true) {
            snakes[i][0] = tiles - 2 - (rand() % 2);
            extremeSnake = false;
        } else if (difficulty == 's') {
            snakes[i][0] = (rand() % (tiles - boardSize * 3 - 1) + boardSize * 3);
        } else {
            snakes[i][0] = (rand() % (tiles - boardSize * 3 - 1)) + boardSize * 2;
        }
        //printf("DEBUG: Snakes Kopf ist nun %i\n", snakes[i][0]);
        if (frontField[snakes[i][0]][0] == 'S') {
            do {
                snakes[i][0] += (rand() % 5) - 2;
                //printf("DEBUG: Snakes Kopf wurde zu %i geändert\n", snakes[i][0]);
            } while (frontField[snakes[i][0]][0] == 'S' || snakes[i][0] >= tiles - 1);
        }
        sprintf(frontField[snakes[i][0]], "S%d", i); //and write it in the Array
        //printf("DEBUG: Snakes Kopf wurde auf Feld %i platziert\n", snakes[i][0] + 1);
        //check if this is the highest snake
        if (snakes[i][0] > highestSnake) {
            //Snake is higher than the others => set the new highestSnake
            highestSnake = snakes[i][0];
        }
        do {
            int maxFall = (snakes[i][0] - 1); // max possible distance backwards
            int minFall = boardSize * 2; // at least 2 rows backwards
            if (maxFall < minFall) {
                minFall = maxFall;
            }
            int fallDistance = (rand() % (maxFall - minFall + 1)) + minFall;
            snakes[i][1] = snakes[i][0] - fallDistance; //generate a random number, at least 2 rows under the snake beginning but still on the board
//            printf("Schlange %i Ende wird an Stelle %i versucht\n", i, snakes[i][1]);
        } while (snakes[i][1] < 1 || frontField[snakes[i][1]][0] == 'S'); //do this til there is a free spot
        sprintf(frontField[snakes[i][1]], "S%d", i); //and write it in the Array
//        printf("Schlange erfolgreich an %i und %i platziert\n", snakes[i][0], snakes[i][1]);
    }


    //LADDERS
    //Calculate Ladder quantity
    if (difficulty == 'e') {
        amountL = 0; //No Ladders
    } else if (difficulty == 's') {
        amountL = rand() % (tiles / 45 + 2) + 2; //Less Ladders than normal
    } else {
        amountL = rand() % (tiles / 25 + 2) + 2; //Normal amount of Ladders
    }

    //Create Multidimensional Ladder Array for Start and End of the Ladder
    ladders = calloc(amountL, sizeof(int) * 2);
    if (ladders == NULL) {
        printf("Fehler: Speicher konnte nicht allokiert werden!\n");
//          exit(1);
    }
    //Position Ladders
    for (int i = 0; i < amountL; i++) {
        ladders[i][0] = (rand() % (tiles - boardSize * 3 - 1)) + boardSize * 2;
        //printf("DEBUG: Ladder Top wurde auf Feld %i gerollt\n", ladders[i][0]);
        if (frontField[ladders[i][0]][0] == 'L' || frontField[ladders[i][0]][0] == 'S' || ladders[i][0] > highestSnake) {
            do {
                ladders[i][0] += (rand() % 6) - 3;
                //printf("DEBUG: Ladders Top wurde auf %i verschoben\n", ladders[i][0]);
            } while (frontField[snakes[i][0]][0] == 'L' || frontField[ladders[i][0]][0] == 'S' || ladders[i][0] > highestSnake);
        }
        sprintf(frontField[ladders[i][0]], "L%d", i); //and write it in the Array
        //printf("DEBUG: Ladders Top wurde auf %i platziert\n", ladders[i][0]);
        do {
            int maxFall = (ladders[i][0] - 1); // max possible distance backwards
            int minFall = boardSize * 2; // at least 2 rows backwards
            if (maxFall < minFall) {
                minFall = maxFall;
            }
            int fallDistance = (rand() % (maxFall - minFall + 1)) + minFall;
            ladders[i][1] = ladders[i][0] - fallDistance; //generate a random number, at least 2 rows under the snake beginning but still on the board
//              printf("Leiter %i Ende wird an Stelle %i versucht\n", i, ladders[i][1]);
        } while (ladders[i][1] < 1 || ladders[i][1] >= tiles - 1 || frontField[ladders[i][1]][0] == 'S' || frontField[ladders[i][1]][0] == 'L'); //do this til there is a free spot
        sprintf(frontField[ladders[i][1]], "L%d", i); //and write it in the Array
//          printf("Leiter erfolgreich an %i und %i platziert\n", ladders[i][0], ladders[i][1]);
    }


    //Copy final Board version to background Array
    count = 0;
    for (int j = 0; j < boardSize; j++) {
        for (int i = 1; i < boardSize + 1; i++) {
            strncpy(backField[count], frontField[count], 3);
            backField[count][3] = '\0';
            count++;
        }
    }
}

int main()
{
    //Start Titlescreen
    titleScreen();

    setBoardSize();

    //For Random
    srand(time(NULL) + rand());


    //Array for whole Frontboard
    tiles = boardSize * boardSize;
    //Reserve Memory for this Array
    frontField = calloc(tiles, sizeof(char[4]));
    if (frontField == NULL) {
        printf("Fehler: Speicher konnte nicht allokiert werden!\n");
        exit(1);
    }

    //Reserve Memory for this Array
    backField = calloc(tiles, sizeof(char[4]));
    if (backField == NULL) {
        printf("Fehler: Speicher konnte nicht allokiert werden!");
        exit(1);
    }

    //Build Front Array for the Main Board
    buildBoard();

    //Print First Time for Player to see the Boardsize
    printf("Dein Spielfeld wird nun erstellt");
    for (int i = 0; i < 3; i++) {
        printf(".");
        Sleep(300);
    }
    printBoard();

    //Difficulty Settings
    setDifficulty();

    //Place Snakes and Ladders
    setObjectives();

    //Simulate Calculations
    printf("\n\nPlatziere Schlangen und Leitern");
    for (int i = 0; i < 6; i++) {
        printf(".");
        Sleep(600);
    }
    printBoard();

    //Set Player on Start
    frontField[playerPos][0] = ' '; //First Digit
    frontField[playerPos][1] = '@'; //Second Digit

    printf("\n\nPlatziere Spieler");
    for (int i = 0; i < 3; i++) {
        printf(".");
        Sleep(100);
    }

    printBoard();

    //Game-Loop
    while(1) {

        rollDice();

        movePlayer();

        checkPos();

    }


    return 0;
}
