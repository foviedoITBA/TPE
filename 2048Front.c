#include <stdio.h>
#include <stdlib.h>

#include "2048Tipos.h"
#include "2048Back.h"
#include "getnum.h"

#define CANT_OPCIONES_MENU 3
#define CANT_DIFICULTADES 3
#define BORRA_BUFFER() while(getchar() != '\n')
#define opcionValida(opcion, cantidad) ((opcion) > 0 && (opcion) <= (cantidad) ? 1 : 0)
#define imprimirData(DIF) printf("Tablero %dx%d\nFicha ganadora: %d\nUndos: %d\n\n", TAMANIO_ ## DIF ,TAMANIO_ ## DIF ,VICTORIA_ ## DIF ,UNDOS_ ## DIF)

void limpiarPantalla();
int leerOpcion(int);
int menuPrincipal(INFO*);
void menuDificultad(INFO*);
void menuCargar(INFO*);

int main(void)
{
	INFO infoActual, infoRespaldo;
	int opcion = menuPrincipal(&infoActual);
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

int leerOpcion(int cantidad)
{
	int opcion;	
	do
	{
		opcion = getint("Ingrese opción: ");
		if (!opcionValida(opcion, cantidad))
			printf("Opción inválida\n");
	} while(!opcionValida(opcion, cantidad));
	return opcion;
}

int menuPrincipal(INFO * laInfo)
{
	int opcion = 0;
	limpiarPantalla();
	printf("*****¡Bienvenido al 2048!*****\n\n");
	
	printf("1: Juego Nuevo\n");
	printf("2: Cargar Juego\n");
	printf("3: Salir\n\n");
	
	opcion = leerOpcion(CANT_OPCIONES_MENU);
	
	switch(opcion)
	{
		case 1:
			menuDificultad(laInfo);
			break;
		case 2:
			menuCargar(laInfo);
			break;
		case 3:
			break;
	}

	return opcion;
}

void menuDificultad(INFO * laInfo)
{
	int opcion;

	limpiarPantalla();
	printf("*****Juego Nuevo*****\n\n");
	printf("1: Fácil\n");
	imprimirData(FACIL);
	printf("2: Medio\n");
	imprimirData(MEDIO);
	printf("3: Difícil\n");
	imprimirData(DIFICIL);
	
	laInfo->dificultad = leerOpcion(CANT_DIFICULTADES);
}

void menuCargar(INFO * laInfo)
{
	printf("Ingrese el nombre del archivo: ");
	/*laInfo->nombreArchivoCarga = leerNombreArchivo();*/
}


