#ifndef BACK_H
#define BACK_H

#include <stdlib.h>
#include <stdio.h>		/* Esta aqui pues 2048Back.c utiliza funciones de archivo, definidas en stdio. */ 
#include <time.h>

#define VALOR_FICHA_NUEVA()	((rand() / (double) RAND_MAX) <= 0.89 ? 2 : 4)	/*	Retorna 2 o 4, los valores que puede tomar una ficha nueva.	*/
#define randomizeSeed()	srand(time(0))

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

#define ARRIBA 		'w'
#define ABAJO 		's'
#define IZQUIERDA 	'a'
#define DERECHA 	'd'
#define UNDO	 	'u'
#define GUARDAR	 	'g'
#define QUIT 		'q'

#define MAX_JUGADAS	5	/*4 direcciones y undo.*/

typedef enum {OK = 0, ERROR_MEMORIA = -1, ERROR_ARCHIVO = -2} Cod_Error;
typedef enum {FALSE = 0, TRUE} BOOL;

typedef enum {NADA, NUMERO} Estado;

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
	char * nombreArchivo;
	BOOL ganaste;
} Info;

unsigned short int dameTamanio(unsigned int);
Cod_Error prepararJuegoNuevo(Info*, Info*);
Cod_Error cargarPartida(Info *, Info *);
Cod_Error actualizarInfo(Info*,Info*, char);
unsigned short int validarJugadas(Info*);
unsigned short int dameVictoria(unsigned short int);
void guardaPartida(Info * laInfo);

#endif

