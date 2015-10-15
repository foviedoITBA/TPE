#include <stdlib.h>
#include <time.h>

#define fichaNueva ((rand() / (double) RAND_MAX) <= 0.89 ? 2 : 4)

typedef unsigned short int FICHA
typedef **FICHA TABLERO
typedef unsigned short int DIFICULTAD
typedef unsigned int PUNTAJE

typedef struct
{
	DIFICULTAD dificultad;
	PUNTAJE puntaje;
	char undos;
	TABLERO tablero;
} INFO;


