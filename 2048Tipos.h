#ifndef TIPOS_H
#define TIPOS_H

enum {FACIL = 1, MEDIO, DIFICIL};

typedef unsigned short int FICHA;
typedef unsigned short int** TABLERO;
typedef unsigned short int DIFICULTAD;
typedef unsigned int PUNTAJE;

typedef struct
{
	DIFICULTAD dificultad;
	PUNTAJE puntaje;
	char undos;
	TABLERO tablero;
} INFO;

#endif
