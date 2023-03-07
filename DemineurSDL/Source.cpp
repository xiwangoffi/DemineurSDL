#include "SDL.h"
#undef main
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <SDL_image.h>

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

void reset(int size, int SQUARE_SIZE, SDL_Rect rect, int* trappedBoard, int* playableBoard, int nbBomb, SDL_Renderer* renderer);
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


void SDL(int size, int* trappedBoard, int* playableBoard, int nbBomb){
	int SQUARE_SIZE = 60;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	//bool board[WINDOW_WIDTH / SQUARE_SIZE][WINDOW_HEIGHT / SQUARE_SIZE] = { 0 };

	// Initialiser SDL
	SDL_Init(SDL_INIT_VIDEO);

	IMG_Init(IMG_INIT_JPG);
    
	// Cr�er la fen�tre
	if (size > 15) {
		SQUARE_SIZE = 30;
	}
	SDL_Rect rect = { 0, 0, SQUARE_SIZE, SQUARE_SIZE };
	window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size * SQUARE_SIZE, size * SQUARE_SIZE, SDL_WINDOW_SHOWN);


	// Creer le rendu pour dessiner
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Surface* gameOver = IMG_Load("game_over.bmp");
	SDL_Surface* winImg = IMG_Load("win.bmp");
    SDL_Surface* imageB1[10] = { IMG_Load("Pilier1.bmp"), IMG_Load("Pilier2.bmp"), IMG_Load("Pilier3.bmp"), IMG_Load("Pilier4.bmp"), IMG_Load("Pilier5.bmp"), IMG_Load("Pilier6.bmp"), IMG_Load("Pilier7.bmp"), IMG_Load("Pilier8.bmp"), IMG_Load("flagB2.bmp"), IMG_Load("mineB1.bmp") };
	SDL_Surface* imageB2[10] = { IMG_Load("Pilier1_2.bmp"), IMG_Load("Pilier2_2.bmp"), IMG_Load("Pilier3_2.bmp"), IMG_Load("Pilier4_2.bmp"), IMG_Load("Pilier5_2.bmp"), IMG_Load("Pilier6_2.bmp"), IMG_Load("Pilier7_2.bmp"), IMG_Load("Pilier8_2.bmp"), IMG_Load("flagB1.bmp"), IMG_Load("mineB2.bmp") };



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
	int loose = 0;
	int win = 0;
	bool KEYS[322];




	while (true) {

		int currentTime = SDL_GetTicks();

        

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			}else if ((loose == 1 || win == 2) && event.key.keysym.sym == SDLK_SPACE) {
                reset(size, SQUARE_SIZE, rect, trappedBoard, playableBoard, nbBomb, renderer);
				loose = 0;
				win = 0;
				firstPlay = 0;

			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				int x = event.button.x / SQUARE_SIZE + 1;
				int y = event.button.y / SQUARE_SIZE + 1;

				if (event.button.button == SDL_BUTTON_RIGHT && firstPlay == 1 && loose == 0 && win != 2) {
					flagAnswer = 1;
					if (flagAnswer == 1) {
						rect.x = (x - 1) * SQUARE_SIZE;
						rect.y = (y - 1) * SQUARE_SIZE;
						if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 13 && (x + y) % 2 == 0) {
							SDL_Rect dstrect = { (x - 1) * SQUARE_SIZE, (y - 1) * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
							SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageB2[8]);
							SDL_RenderCopy(renderer, texture, NULL, &dstrect);
							SDL_RenderPresent(renderer);
							playVerif(trappedBoard, playableBoard, x, y, flagAnswer, size, nbBomb);
						}
						else if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 13) {
							SDL_Rect dstrect = { (x - 1) * SQUARE_SIZE, (y - 1) * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
							SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageB1[8]);
							SDL_RenderCopy(renderer, texture, NULL, &dstrect);
							SDL_RenderPresent(renderer);
							playVerif(trappedBoard, playableBoard, x, y, flagAnswer, size, nbBomb);
						}
						else if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 12 && (x + y) % 2 == 0) {
							SDL_SetRenderDrawColor(renderer, 162, 209, 73, 255);
							SDL_RenderFillRect(renderer, &rect);
							SDL_RenderPresent(renderer);
                            playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] = 13;
						}
						else if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 12) {
							SDL_SetRenderDrawColor(renderer, 170, 215, 81, 255);
							SDL_RenderFillRect(renderer, &rect);
							SDL_RenderPresent(renderer);
							playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] = 13;
						}
						printBoard(playableBoard, size);
						printBoard(trappedBoard, size);
					}
				}

				else {
					flagAnswer = 0;
				}



                if (playableBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 13 && event.button.button != SDL_BUTTON_RIGHT && loose == 0 &&  win != 2) {

                    if (firstPlay == 0) {
                        bombCreation(trappedBoard, x, y, size, nbBomb);
                        firstPlay = 1;
                    }

                    win = playVerif(trappedBoard, playableBoard, x, y, flagAnswer, size, nbBomb);
                    showArea(trappedBoard, playableBoard, size);
                    printBoard(playableBoard, size);
                    printBoard(trappedBoard, size);

                    if (trappedBoard[((x - 1) + ((y - 1) - 1) * size) + size] == 11) {
                        loose = 1;
                        for (int x2 = 0; x2 < size * SQUARE_SIZE / SQUARE_SIZE; x2++) {
                            for (int y2 = 0; y2 < size * SQUARE_SIZE / SQUARE_SIZE; y2++) {
                                if (trappedBoard[(x2 + (y2 - 1) * size) + size] == 11) {
                                    SDL_Rect dstrect = { x2 * SQUARE_SIZE, y2 * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
                                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageB1[9]);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                                    SDL_RenderPresent(renderer);
                                }
                            }
                        }
                    }
                    for (int x2 = 0; x2 < size * SQUARE_SIZE / SQUARE_SIZE; x2++) {
                        for (int y2 = 0; y2 < size * SQUARE_SIZE / SQUARE_SIZE; y2++) {
                            rect.x = x2 * SQUARE_SIZE;
                            rect.y = y2 * SQUARE_SIZE;
                            if (playableBoard[(x2 + (y2 - 1) * size) + size] == 10 && flagAnswer == 0) {
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

                            for (int i = 0; i < 8; i++)
                            {
                                if ((x2 + y2) % 2 == 0 && playableBoard[(x2 + (y2 - 1) * size) + size] == i + 1) {
                                    SDL_Rect dstrect = { x2 * SQUARE_SIZE, y2 * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
                                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageB2[i]);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                                    SDL_RenderPresent(renderer);
                                }
                                else if (playableBoard[(x2 + (y2 - 1) * size) + size] == i + 1) {
                                    SDL_Rect dstrect = { x2 * SQUARE_SIZE, y2 * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE };
                                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageB1[i]);
                                    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
                                    SDL_RenderPresent(renderer);
                                }
                            }

                        }
                    }
				}
                if (loose == 1){
					SDL_Rect dstrect = { size * SQUARE_SIZE / 6, size * SQUARE_SIZE / 6, size * SQUARE_SIZE / 1.5, size * SQUARE_SIZE / 1.5 };
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, gameOver);
					SDL_RenderCopy(renderer, texture, NULL, &dstrect);
					SDL_RenderPresent(renderer);
				}else if (win == 2) {
					SDL_Rect dstrect = { size * SQUARE_SIZE / 6, size * SQUARE_SIZE / 6, size * SQUARE_SIZE / 1.5, size * SQUARE_SIZE / 1.5 };
					SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, winImg);
					SDL_RenderCopy(renderer, texture, NULL, &dstrect);
					SDL_RenderPresent(renderer);
				}
			}
		}

		previousTime = SDL_GetTicks();
        
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

	// Lib�rer la m�moire
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void reset(int size, int SQUARE_SIZE, SDL_Rect rect, int* trappedBoard, int* playableBoard, int nbBomb, SDL_Renderer* renderer){
	for (int i = 0; i < size * size; i++) {
		trappedBoard[i] = 10;
		playableBoard[i] = 13;
	}
	//int SDL_RenderClear(SDL_Renderer * renderer);
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
			SDL_RenderPresent(renderer);

		}
	}
}

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
    //V�rifie si l'entr�e est un nombre entier
    char entree[100];
    int nombre;
    char* fin;
    while (fgets(entree, 100, stdin) != NULL) {
        // Supprimer le caract�re de retour � la ligne � la fin de l'entr�e
        entree[strcspn(entree, "\n")] = '\0';
        // V�rifier si l'entr�e est un nombre entier valide
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
    if (clue == 0) {  //Affiche vide dans le coin sup�rieur gauche
        printf("\n   ");
    }
    else if (clue == 1) { // Affiche l'indice 1 � gauche
        printf("\n  %d", clue);
    }
    else if (clue < 10) { // Affiche l'indice - 1 et retourne � la ligne pour afficher le suivant (si indice < 10)
        printf(" %d\n  %d", clue - 1, clue);
    }
    else if (clue >= 10) { // Affiche l'indice - 1 et retourne � la ligne pour afficher le suivant (si indice > 10)
        printf(" %d\n %d", clue - 1, clue);
    }
}

void bombCreation(int* trappedBoard, int x, int y, int size, int nbBomb) {
    //Random values init
    int i, random;
    srand(time(NULL));

    //Random Creation Bomb
    for (int i = 1; i <= nbBomb; i++) { // boucle pour la cr�ation de bombes
        random = rand() % (size * size);
        if ((random >= (x + (y - 1) * size) - 2 && random <= x + (y - 1) * size) || (random >= (x + (y - 1) * size) - 2 - size && random <= (x + (y - 1) * size) - size) || (random >= (x + (y - 1) * size) - 2 + size && random <= (x + (y - 1) * size) + size)) { // v�rifie si la bombe peut �tre poser a coter de la ou le joueur veut joueur
            random += 3;
        }
        if (trappedBoard[random] == 11) { // v�rifie si la bombe se pose sur une autre
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
    while (isO == 1) { // S'il y a des O a v�rifier
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
        if (trappedBoard[i] == 11) { // S'il y a une mine, r�-affiche le tableau avec cette mine avec un d�lai
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

    //qu�mande la position a laquelle on veut jouer

    printf("\nIndiquer une colonne entre (1 et %d): ", size);
    x = isInt();
    printf("\nIndiquer une ligne entre (1 et %d): ", size);
    y = isInt();

    while (x > size || x < 1 || y > size || y < 1) { // V�rifie si les valeurs introduites entrent dans le tableau
        printf("\nValeurs non valide\n");
        printf("\nIndiquer une colonne entre (1 et %d): ", size);
        x = isInt();
        printf("\nIndiquer une ligne entre (1 et %d): ", size);
        y = isInt();
    }

    bombCreation(trappedBoard, x, y, size, nbBomb); // Cr�er les bombes



    playableBoard[(x + (y - 1) * size) - 1] = 10; // Joue � la position qu�mand�e

    showArea(trappedBoard, playableBoard, size); // D�couvre les cases vides

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

    //qu�mande la position au joueur

    printf("\nVoulez-vous mettre un drapeau ? yes = 1 or no = 0\n");
    answer = isInt();

    printf("\nIndiquer une colonne entre (1 et %d): ", size);
    x = isInt();
    printf("\nIndiquer une ligne entre (1 et %d): ", size);
    y = isInt();

    while (x > size || x < 1 || y > size || y < 1) { // V�rifie si les valeurs introduites entrent dans le tableau
        printf("\nValeurs non valide\n");
        printf("\nIndiquer une colonne entre (1 et %d): ", size);
        x = isInt();
        printf("\nIndiquer une ligne entre (1 et %d): ", size);
        y = isInt();
    }


    isBomb = playVerif(trappedBoard, playableBoard, x, y, answer, size, nbBomb); // V�rifie si on peut jouer a cette position

    showArea(trappedBoard, playableBoard, size); // D�couvre les cases vides

    //Boards print
    system("cls");
    printf("\nMinesweeper :\n");
    printBoard(playableBoard, size);

    return isBomb;
}

void printBoard(int* board, int size) {
    int divide = 0;
    for (int i = 0; i < size * size; i++) {

        if (i == 0) { // si on est au d�but de la boucle affiche les chiffres indicatifs du haut
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

        if (i / size == divide) { // Si on est � la fin d'une ligne, affiche les nombres indicatifs lat�raux et la case � c�t�
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
            if (playableBoard[i] == 12 || playableBoard[i] == 13) { // V�rifie le nombre de drapeau et de cases non d�couvertes
                victory++;
            }
        }
        if (victory == nbBomb) { // S'il y a autant de drapeaux que de cases non d�couvertes s'est gagner
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
                if (playableBoard[i] == 12 || playableBoard[i] == 13) { // V�rifie le nombre de drapeau et de cases non d�couvertes
                    victory++;
                }
            }
            if (victory == nbBomb) { // S'il y a autant de drapeaux que de cases non d�couvertes s'est gagner
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

    // Qu�mande la taille du tableau au joueur

    printf("Taille entre 6 et 30 (un entier pls): ");
    size = isInt();

    while (size < 6 || size > 30) { // V�rifie si la valeur input est < � 5 ou > � 30
        printf("\nValeur non valide \nTaille entre 6 et 30 (un entier pls): ");
        size = isInt();
    }

    // Qu�mande le nombres de bombes

    printf("Nombres de bombes entre 1 et %d (un entier pls): ", (size * size) - 10);
    nbBomb = isInt();

    while (nbBomb < 1 || nbBomb >(size * size) - 10) {
        printf("\nValeur non valide \nNombres de bombes entre 1 et %d (un entier pls): ", (size * size) - 10);
        nbBomb = isInt();
    }

    // Cr�� le tableau

    int* playableBoard = (int*)malloc(sizeof(int) * (size * size));
    int* trappedBoard = (int*)malloc(sizeof(int) * (size * size));

    // Rempli le tableau

    for (int i = 0; i < size * size; i++) {
        trappedBoard[i] = 10;
        playableBoard[i] = 13;
    }

    SDL(size, trappedBoard, playableBoard, nbBomb);

    // Premi�re fonction pour jouer

    prePlay(trappedBoard, playableBoard, size, nbBomb);

    while (booleanLoose == 0) { // S'il n'y a rien, continu�
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