#include <stdio.h>
#include <stdlib.h>

#include "2048Tipos.h"
#include "2048Back.h"

#define MAX_OPCIONES '3'
#define BORRA_BUFFER() while(getchar() != '\n')
#define opcionValida(opcion) ((opcion) > '0' && (opcion) <= MAX_OPCIONES ? 1 : 0)

void limpiarPantalla();
int menu(INFO *);
void juegoNuevo(INFO*);
void cargarJuego(INFO*);


int main(void)
{
	INFO infoActual, infoRespaldo;
	int opcion = menu(&infoActual);
	return 0;
}

void limpiarPantalla()
{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

int menu(INFO * laInfo)
{
	int leer = 0;
	limpiarPantalla();
	printf("*****¡Bienvenido al 2048!*****\n\n");
	do
	{
		printf("1: Juego Nuevo\n");
		printf("2: Cargar Juego\n");
		printf("3: Salir\n\n");
		printf("Ingrese opción: ");	
		
		leer = getchar();
		BORRA_BUFFER();
		
		switch(leer)
		{
			case '1':
				juegoNuevo(laInfo);
				break;
			case '2':
				cargarJuego(laInfo /*, laOtraInfo */);
				break;
			case '3':
				break;
			default:
				printf("Opción inválida\n\n");

		}

	} while(!opcionValida(leer));

	return leer;
}


