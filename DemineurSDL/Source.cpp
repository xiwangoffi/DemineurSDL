

#include "SDL.h"
#undef main
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 750

//don't do this, this is just an example
SDL_Renderer* renderer;
SDL_Window* window;
bool isRunning;
bool fullscreen;
void handleEvents();
void update();
void render();

void printNumbers(int clue);
void printBoard(int* board, int size);
int playVerif(int* trappedBoard, int* playableBoard, int x, int y, int flag, int size, int nbBomb);
void showArea(int* trappedBoard, int* playableBoard, int size);
void bombCreation(int* trappedBoard, int x, int y, int size, int nbBomb);
int play(int* trappedBoard, int* playableBoard, int size, int nbBomb);
void boardCreation(int* trappedBoard, int* playableBoard);
void prePlay(int* trappedBoard, int* playableBoard, int size, int nbBomb);
void showMine(int* trappedBoard, int* playableBoard, int size);
int main();
void red();
void white();
void green();
void cyan();
void yellow();
void blue();
void setColor(int* board, int i);

//handles any events that SDL noticed.
void handleEvents() {
	//the only event we'll check is the  SDL_QUIT event.
	SDL_Event event;
	SDL_PollEvent(&event);


	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

//simple render function
void render() {
	SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
	SDL_RenderClear(renderer);

	//your stuff to render would typically go here.
	SDL_RenderPresent(renderer);
}

//simple update function
void update() {
    //if things could update the code would go in here.
}

void red() {
    printf("\033[0;31m"); // couleur rouge
}

void white() {
    printf("\033[0;37m"); // couleur blanche
}

void green() {
    printf("\033[0;32m"); // couleur verte
}

void cyan() {
    printf("\033[0;36m"); // couleur cyan
}

void yellow() {
    printf("\033[0;33m"); // couleur jaune
}

void magenta() {
    printf("\033[0;35m"); // couleur magenta
}

void blue() {
    printf("\033[0;34m"); // couleur bleu
}

void setColor(int* board, int i) {
    if (board[i] == 1 || board[i] == 2) { // si le nombre de mine est de 1 ou 2 mettre la couleur vert
        green();
    }
    else if (board[i] == 3 || board[i] == 4) { // si le nombre de mine est de 3 ou 4 mettre la couleur jaune
        yellow();
    }
    else if (board[i] == 5 || board[i] == 6) { // si le nombre de mine est de 5 ou 6 mettre la couleur magenta
        magenta();
    }
    else if (board[i] == 7 || board[i] == 8) { // si le nombre de mine est de 7 ou 8 mettre la couleur rouge
        red();
    }
}

int isInt() {
    //Vérifie si l'entrèe est un nombre entier
    char entree[100];
    int nombre;
    char* fin;
    while (fgets(entree, 100, stdin) != NULL) {
        // Supprimer le caractère de retour à la ligne à la fin de l'entrée
        entree[strcspn(entree, "\n")] = '\0';
        // Vérifier si l'entrée est un nombre entier valide
        if (isdigit(entree[0]) || entree[0] == '+' || entree[0] == '-') {
            nombre = strtol(entree, &fin, 10);
            if (*fin == '\0') {
                return nombre;
                break;
            }
        }
        printf("Erreur : veuillez entrer un nombre entier valide.\n");
        printf("Entrez un nombre entier : ");
    }
}

void printNumbers(int clue) {
    if (clue == 0) {  //Affiche vide dans le coin supérieur gauche
        printf("\n   ");
    }
    else if (clue == 1) { // Affiche l'indice 1 à gauche
        printf("\n  %d", clue);
    }
    else if (clue < 10) { // Affiche l'indice - 1 et retourne à la ligne pour afficher le suivant (si indice < 10)
        printf(" %d\n  %d", clue - 1, clue);
    }
    else if (clue >= 10) { // Affiche l'indice - 1 et retourne à la ligne pour afficher le suivant (si indice > 10)
        printf(" %d\n %d", clue - 1, clue);
    }
}

void bombCreation(int* trappedBoard, int x, int y, int size, int nbBomb) {
    //Random values init
    int i, random;
    srand(time(NULL));

    //Random Creation Bomb
    for (int i = 1; i <= nbBomb; i++) { // boucle pour la création de bombes
        random = rand() % (size * size);
        if ((random >= (x + (y - 1) * size) - 2 && random <= x + (y - 1) * size) || (random >= (x + (y - 1) * size) - 2 - size && random <= (x + (y - 1) * size) - size) || (random >= (x + (y - 1) * size) - 2 + size && random <= (x + (y - 1) * size) + size)) { // vérifie si la bombe peut être poser a coter de la ou le joueur veut joueur
            random += 3;
        }
        if (trappedBoard[random] == 11) { // vérifie si la bombe se pose sur une autre
            i--;
        }
        trappedBoard[random] = 11;// pose la mine
    }

    //Verif Number Bomb
    for (int i = 0; i < size * size; i++)
    {
        int numberBomb = 0;
        if (trappedBoard[i] != 11) {

            //Verify Bomb Right
            if (trappedBoard[i + 1] == 11 && (i + 1) % size != 0) {
                numberBomb++;
            }
            //Verify Bomb Left
            if (trappedBoard[i - 1] == 11 && (i - 1) % size != size - 1) {
                numberBomb++;
            }
            //Verify Bomb Up
            if (trappedBoard[i - size] == 11 && i >= size) {
                numberBomb++;
            }
            //Verify Bomb Down
            if (trappedBoard[i + size] == 11 && i <= (size * size) - size) {
                numberBomb++;
            }

            //Verify Bomb Right Up
            if (trappedBoard[i - (size - 1)] == 11 && i >= size && (i + 1) % size != 0 && i != 0) {
                numberBomb++;
            }
            //Verify Bomb Left Up
            if (trappedBoard[i - (size + 1)] == 11 && i >= size && (i - 1) % size != size - 1) {
                numberBomb++;
            }
            //Verify Bomb Right Down
            if (trappedBoard[i + (size + 1)] == 11 && i <= (size * size) - size && (i + 1) % size != 0) {
                numberBomb++;
            }
            //Verify Bomb Left Down
            if (trappedBoard[i + (size - 1)] == 11 && i <= (size * size) - size && (i - 1) % size != size - 1 && i != 0) {
                numberBomb++;
            }

            //Verify if is Bomb
            if (numberBomb != 0)
            {
                trappedBoard[i] = numberBomb;
            }
        }
    }
}

void showArea(int* trappedBoard, int* playableBoard, int size) {
    int isO = 1;
    while (isO == 1) { // S'il y a des O a vérifier
        isO = 0;
        for (int i = 0; i < size * size; i++)
        {
            if (playableBoard[i] == 13 || playableBoard[i] == 12) // Si c'est du vide
            {
                //Verify O Right
                if (playableBoard[i + 1] == 10 && (i + 1) % size != 0) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Left
                else if (playableBoard[i - 1] == 10 && (i - 1) % size != size - 1) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Up
                else if (playableBoard[i - size] == 10 && i >= size) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Down
                else if (playableBoard[i + size] == 10 && i <= (size * size) - size) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }

                //Verify O Right Up
                else if (playableBoard[i - (size - 1)] == 10 && i >= size && (i + 1) % size != 0 && i != 0) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Left Up
                else if (playableBoard[i - (size + 1)] == 10 && i >= size && (i - 1) % size != size - 1) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Right Down
                else if (playableBoard[i + (size + 1)] == 10 && i <= (size * size) - size && (i + 1) % size != 0) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }
                //Verify O Left Down
                else if (playableBoard[i + (size - 1)] == 10 && i <= (size * size) - size && (i - 1) % size != size - 1 && i != 0) {
                    playableBoard[i] = trappedBoard[i];
                    isO = 1;
                }

            }
        }
    }
}

void showMine(int* trappedBoard, int* playableBoard, int size) {
    for (int i = 0; i < size * size; i++) {
        if (trappedBoard[i] == 11) { // S'il y a une mine, ré-affiche le tableau avec cette mine avec un délai
            Sleep(400);
            system("cls");
            playableBoard[i] = 11;
            printBoard(playableBoard, size);
        }
    }
}

void prePlay(int* trappedBoard, int* playableBoard, int size, int nbBomb) {
    int x;
    int y;

    //quémande la position a laquelle on veut jouer

    printf("\nIndiquer une colonne entre (1 et %d): ", size);
    x = isInt();
    printf("\nIndiquer une ligne entre (1 et %d): ", size);
    y = isInt();

    while (x > size || x < 1 || y > size || y < 1) { // Vérifie si les valeurs introduites entrent dans le tableau
        printf("\nValeurs non valide\n");
        printf("\nIndiquer une colonne entre (1 et %d): ", size);
        x = isInt();
        printf("\nIndiquer une ligne entre (1 et %d): ", size);
        y = isInt();
    }

    bombCreation(trappedBoard, x, y, size, nbBomb); // Créer les bombes



    playableBoard[(x + (y - 1) * size) - 1] = 10; // Joue à la position quémandée

    showArea(trappedBoard, playableBoard, size); // Découvre les cases vides

    //Boards print
    system("cls");
    printf("\nMinesweeper :\n");
    printBoard(playableBoard, size);

}

int play(int* trappedBoard, int* playableBoard, int size, int nbBomb) {
    int x;
    int y;
    int flag = 0;
    int isBomb;

    int answer = 0;

    //quémande la position au joueur

    printf("\nVoulez-vous mettre un drapeau ? yes = 1 or no = 0\n");
    answer = isInt();

    printf("\nIndiquer une colonne entre (1 et %d): ", size);
    x = isInt();
    printf("\nIndiquer une ligne entre (1 et %d): ", size);
    y = isInt();

    while (x > size || x < 1 || y > size || y < 1) { // Vérifie si les valeurs introduites entrent dans le tableau
        printf("\nValeurs non valide\n");
        printf("\nIndiquer une colonne entre (1 et %d): ", size);
        x = isInt();
        printf("\nIndiquer une ligne entre (1 et %d): ", size);
        y = isInt();
    }


    isBomb = playVerif(trappedBoard, playableBoard, x, y, answer, size, nbBomb); // Vérifie si on peut jouer a cette position

    showArea(trappedBoard, playableBoard, size); // Découvre les cases vides

    //Boards print
    system("cls");
    printf("\nMinesweeper :\n");
    printBoard(playableBoard, size);

    return isBomb;
}

void printBoard(int* board, int size) {
    int divide = 0;
    for (int i = 0; i < size * size; i++) {

        if (i == 0) { // si on est au début de la boucle affiche les chiffres indicatifs du haut
            for (int j = 0; j <= size; j++) {
                if (j == 0) {
                    printf("   ");
                }
                else if (j < 10) {
                    printf("  %d", j);
                }
                else if (j >= 10) {
                    printf(" %d", j);
                }

            }
        }

        if (i / size == divide) { // Si on est à la fin d'une ligne, affiche les nombres indicatifs latéraux et la case à côté
            divide++;
            if (board[i] == 10) {
                printNumbers(divide);
                white();
                printf("   ");
            }
            else if (board[i] == 11) {
                printNumbers(divide);
                cyan();
                printf("  X");
                white();
            }
            else if (board[i] == 12) {
                printNumbers(divide);
                blue();
                printf("  F");
                white();
            }
            else if (board[i] == 13) {
                printNumbers(divide);
                white();
                printf("  .");
            }
            if (board[i] < 10)
            {
                printNumbers(divide);
                setColor(board, i);
                printf("  %d", board[i]);
                white();
            }
        }
        else { // Affiche la case
            if (board[i] == 10) {
                white();
                printf("   ");
            }
            else if (board[i] == 11) {
                cyan();
                printf("  X");
                white();
            }
            else if (board[i] == 12) {
                blue();
                printf("  F");
                white();
            }
            else if (board[i] == 13) {
                white();
                printf("  .");
            }
            if (board[i] < 10)
            {
                setColor(board, i);
                printf("  %d", board[i]);
                white();
            }
        }
        if (i == (size * size) - 1) { // Affiche les nombres indicatifs du bas
            for (int j = 0; j <= size; j++) {
                if (j == 0) {
                    printf(" %d\n   ", size);
                }
                else if (j < 10) {
                    printf("  %d", j);
                }
                else if (j >= 10) {
                    printf(" %d", j);
                }

            }
        }
    }
}

int playVerif(int* trappedBoard, int* playableBoard, int x, int y, int flag, int size, int nbBomb) {
    int victory = 0;

    if (flag == 1 && playableBoard[(x + (y - 1) * size) - 1] == 13) { // Si on peut jouer un drapeau
        playableBoard[(x + (y - 1) * size) - 1] = 12;
        for (int i = 0; i < size * size; i++)
        {
            if (playableBoard[i] == 12 || playableBoard[i] == 13) { // Vérifie le nombre de drapeau et de cases non découvertes
                victory++;
            }
        }
        if (victory == nbBomb) { // S'il y a autant de drapeaux que de cases non découvertes s'est gagner
            return 2;
        }
        else { // sinon rien
            return 0;
        }
    }
    else if (flag == 1) { // Si on ne peut pas jouer le drapeau il ne se passe rien 
        return 0;
    }
    else {
        if (trappedBoard[(x + (y - 1) * size) - 1] == 11) { // S'il y a une bombe s'est perdu
            printf("\t\nBomb right there");
            return 1;
        }
        else { // sinon s'est pas perdu
            printf("\t\nOnly luck");
            playableBoard[(x + (y - 1) * size) - 1] = trappedBoard[(x + (y - 1) * size) - 1];
            for (int i = 0; i < size * size; i++)
            {
                if (playableBoard[i] == 12 || playableBoard[i] == 13) { // Vérifie le nombre de drapeau et de cases non découvertes
                    victory++;
                }
            }
            if (victory == nbBomb) { // S'il y a autant de drapeaux que de cases non découvertes s'est gagner
                return 2;
            }
            else { // sinon rien
                return 0;
            }
        }

    }

}

int main() {

    //The sense of life
    std::cout << "Minesweeper of BeboUwU!\n";

    //init win/lose
    int booleanLoose = 0;

    //Boards init

    int size = 0;
    int nbBomb = 0;

    // Quémande la taille du tableau au joueur

    printf("Taille entre 6 et 30 (un entier pls): ");
    size = isInt();

    while (size < 6 || size > 30) { // Vérifie si la valeur input est < à 5 ou > à 30
        printf("\nValeur non valide \nTaille entre 6 et 30 (un entier pls): ");
        size = isInt();
    }

    // Quémande le nombres de bombes

    printf("Nombres de bombes entre 1 et %d (un entier pls): ", (size * size) - 10);
    nbBomb = isInt();

    while (nbBomb < 1 || nbBomb >(size * size) - 10) {
        printf("\nValeur non valide \nNombres de bombes entre 1 et %d (un entier pls): ", (size * size) - 10);
        nbBomb = isInt();
    }

    // Créé le tableau

    int* playableBoard = (int*)malloc(sizeof(int) * (size * size));
    int* trappedBoard = (int*)malloc(sizeof(int) * (size * size));

    // Rempli le tableau

    for (int i = 0; i < size * size; i++) {
        trappedBoard[i] = 10;
        playableBoard[i] = 13;
    }


    int SQUARE_SIZE = 50;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    //bool board[WINDOW_WIDTH / SQUARE_SIZE][WINDOW_HEIGHT / SQUARE_SIZE] = { 0 };

    // Initialiser SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Créer la fenêtre
    if (size > 15) {
        SQUARE_SIZE = 30;
    }
    SDL_Rect rect = { 0, 0, SQUARE_SIZE, SQUARE_SIZE };
    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size * SQUARE_SIZE, size * SQUARE_SIZE, SDL_WINDOW_SHOWN);


    // Créer le rendu pour dessiner
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Changer la couleur du fond en blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Dessiner le damier
    for (int x = 0; x < size * SQUARE_SIZE / SQUARE_SIZE; x++) {
        for (int y = 0; y < size * SQUARE_SIZE / SQUARE_SIZE; y++) {
            rect.x = x * SQUARE_SIZE;
            rect.y = y * SQUARE_SIZE;

            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 162, 209, 73, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 170, 215, 81, 255);
            }

            SDL_RenderFillRect(renderer, &rect);

            // Initialiser le damier avec toutes les cases visibles
            //board[x][y] = true;
        }
    }

    // Afficher le rendu
    SDL_RenderPresent(renderer);

    // Boucle principale
	SDL_Event event;

	int firstPlay = 0;
	int flagAnswer = 0;
	int frameCount = 0;
	int startTime = SDL_GetTicks();
    int previousTime = SDL_GetTicks();
	float fps = 0.0;
    int fpsLimit = 60;

	while (true) {

		int currentTime = SDL_GetTicks();
		
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x / SQUARE_SIZE + 1;
                int y = event.button.y / SQUARE_SIZE + 1;
                if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 13) {

                    if (firstPlay == 0) {
                        bombCreation(trappedBoard, x, y, size, nbBomb);
                        firstPlay = 1;
                    }else{
                        if(event.button.button == SDL_BUTTON_RIGHT){
                            flagAnswer = 1;
                        }else{
                            flagAnswer = 0;
                        }
                    }

					playVerif(trappedBoard, playableBoard, x, y, flagAnswer, size, nbBomb);
					showArea(trappedBoard, playableBoard, size);
					printBoard(playableBoard, size);
					printBoard(trappedBoard, size);

                    for (int x2 = 0; x2 < size * SQUARE_SIZE / SQUARE_SIZE; x2++) {
                        for (int y2 = 0; y2 < size * SQUARE_SIZE / SQUARE_SIZE; y2++) {
                            rect.x = x2 * SQUARE_SIZE;
                            rect.y = y2 * SQUARE_SIZE;
                            if ((x2 + y2) % 2 == 0 && playableBoard[(x2 + (y2 - 1) * size) + size] != 13) {
                                SDL_SetRenderDrawColor(renderer, 215, 184, 153, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_RenderPresent(renderer);
                            }
                            else if (playableBoard[(x2 + (y2 - 1) * size) + size] != 13) {
                                SDL_SetRenderDrawColor(renderer, 229, 194, 159, 255);
                                SDL_RenderFillRect(renderer, &rect);
                                SDL_RenderPresent(renderer);
                            }

                        }
                    }

                }
            }
        }

		previousTime = SDL_GetTicks();

		printf("\n %d", (1000 / fpsLimit) - (currentTime - previousTime));

		Sleep((1000 / fpsLimit) - (currentTime - previousTime));

		frameCount++;
		if (currentTime > startTime + 1000) {
			int elapsedTime = currentTime - startTime;
			fps = ((float)frameCount / elapsedTime) * 1000.0;
			printf("FPS: %.2f\n", fps);
			frameCount = 0;
			startTime = currentTime;
		}

    }

    // Libérer la mémoire
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // Première fonction pour jouer

    prePlay(trappedBoard, playableBoard, size, nbBomb);

    while (booleanLoose == 0) { // S'il n'y a rien, continué
        booleanLoose = play(trappedBoard, playableBoard, size, nbBomb);
    }
    if (booleanLoose == 2) { // Si gagner s'est gagner
        showMine(trappedBoard, playableBoard, size);
        printf("\n\n");
        green();
        printf("  /$$$$$$                                         \n");
        printf(" /$$__  $$                                        \n");
        printf("| $$  \\__/  /$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$ \n");
        printf("| $$ /$$$$ |____  $$| $$__  $$ /$$__  $$ /$$__  $$\n");
        printf("| $$|_  $$  /$$$$$$$| $$  \\ $$| $$$$$$$$| $$  \\__/\n");
        printf("| $$  \\ $$ /$$__  $$| $$  | $$| $$_____/| $$      \n");
        printf("|  $$$$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$      \n");
        printf(" \\______/  \\_______/|__/  |__/ \\_______/|__/      \n");
        white();



    }
    else if (booleanLoose == 1) { // Si perdu s'est perdu et affiche les bombes
        showMine(trappedBoard, playableBoard, size);
        printf("\n\n");
        red();
        printf("     /$$$$$$$                 /$$          \n");
        printf("    | $$__  $$               | $$          \n");
        printf("    | $$  \\ $$ /$$$$$$   /$$$$$$$ /$$   /$$\n");
        printf("    | $$$$$$$//$$__  $$ /$$__  $$| $$  | $$\n");
        printf("    | $$____/| $$$$$$$$| $$  | $$| $$  | $$\n");
        printf("    | $$     | $$_____/| $$  | $$| $$  | $$\n");
        printf("    | $$     |  $$$$$$$|  $$$$$$$|  $$$$$$/\n");
        printf("    |__/      \\_______/ \\_______/ \\______/ \n");
        white();
    }

}