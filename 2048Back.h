#ifndef BACK_H
#define BACK_H

#include <stdlib.h>
#include <time.h>

#define FICHA_NUEVA() ((rand() / (double) RAND_MAX) <= 0.89 ? 2 : 4)

#define UNDOS_FACIL 8
#define UNDOS_MEDIO 4
#define UNDOS_DIFICIL 2

#define TAMANIO_FACIL 8
#define TAMANIO_MEDIO 6
#define TAMANIO_DIFICIL 4

#define VICTORIA_FACIL 1024
#define VICTORIA_MEDIO 2048
#define VICTORIA_DIFICIL 2048

#define MAX_FILE_NAME_SIZE 51

typedef enum {OK = 0, ERROR_MEMORIA = -1, ERROR_ARCHIVO = -2} Cod_Error;

enum {JUEGO_NUEVO = 1, CARGAR_JUEGO, SALIR};
enum {DIF_FACIL = 1, DIF_MEDIO, DIF_DIFICIL};

typedef unsigned short int Ficha;
typedef Ficha** Tablero;
typedef unsigned short int Dificultad;
typedef unsigned int Puntaje;

typedef struct
{
	Dificultad dificultad;
	Puntaje puntaje;
	char undos;
	Tablero tablero;
	char * nombreArchivoCarga;
} Info;

unsigned short int dameTamanio(Dificultad);
Cod_Error prepararJuego(Info*, Info*, int);
static Cod_Error inicializarNuevo(Info *);
static Tablero crearTablero(unsigned short int);
static void liberarTablero(Info*);

#endif