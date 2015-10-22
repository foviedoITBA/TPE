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

#define ARRIBA 'w'
#define ABAJO 's'
#define IZQUIERDA 'a'
#define DERECHA 'd'
#define QUIT 'q'
#define GUARDAR 'g'
#define UNDO 'u'

#define MAX_JUGADAS 5 /*4 direcciones y undo*/

typedef enum {OK = 0, ERROR_MEMORIA = -1, ERROR_ARCHIVO = -2} Cod_Error;
typedef enum {FALSE = 0, TRUE} BOOL;

enum {JUEGO_NUEVO = 1, CARGAR_JUEGO, SALIR};
enum {DIF_FACIL = 1, DIF_MEDIO, DIF_DIFICIL};

typedef unsigned short int Ficha;
typedef Ficha** Tablero;
typedef unsigned int Puntaje;
typedef unsigned short int Tamanio;

typedef struct 
{
	unsigned short int x;
	unsigned short int y;
} posicionLibre;

typedef struct
{
	Tamanio tamanio;
	Puntaje puntaje;
	unsigned short int undos;
	BOOL undoPosible;
	Tablero tablero;
	char jugadasValidas[MAX_JUGADAS];
	char * nombreArchivoCarga;
} Info;

unsigned short int dameTamanio(unsigned int);
Cod_Error prepararJuego(Info*, Info*, int);
void actualizarInfo(Info*,Info*, char);
unsigned short int validarJugadas(Info*);


#endif
