#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "2048Back.h"
#include "getnum.h"

#define CANT_OPCIONES_MENU 3
#define CANT_DIFICULTADES 3
#define BORRA_BUFFER() while(getchar() != '\n')
#define opcionValida(opcion, cantidad) ((opcion) > 0 && (opcion) <= (cantidad) ? 1 : 0)
#define imprimirData(DIF) printf("Tablero %dx%d\nFicha ganadora: %d\nUndos: %d\n\n", TAMANIO_ ## DIF ,TAMANIO_ ## DIF ,VICTORIA_ ## DIF ,UNDOS_ ## DIF)

void limpiarPantalla();
int leerOpcion(int);
int menuPrincipal(Info*);
void menuDificultad(Info*);
void menuCargar(Info*);
char * leerNombreArchivo();

void verQueOnda(Info * laInfo)
{
	int i, j;
	unsigned short int tamanio;
	tamanio = dameTamanio(laInfo->dificultad);
	limpiarPantalla();
	printf("\n");
	for (i = 0; i < tamanio; i++)
	{
		for (j = 0; j < tamanio; j++)
			printf("%hu", laInfo->tablero[i][j]);
		printf("\n");
	}
	printf("\n\n");

	printf("Undos: %hu\nPuntaje: %u\n", laInfo->undos, laInfo->puntaje);
}

int main(void)
{
	Info infoActual, infoRespaldo;
	int opcion = menuPrincipal(&infoActual);
	if (opcion == SALIR)
	{
		printf("*****¡Adiós!*****");
		return 0;
	}
	prepararJuego(&infoActual, &infoRespaldo, opcion);


	verQueOnda(&infoActual);
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

int menuPrincipal(Info * laInfo)
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
		case JUEGO_NUEVO:
			menuDificultad(laInfo);
			break;
		case CARGAR_JUEGO:
			menuCargar(laInfo);
			break;
		case SALIR:
			break;
	}

	return opcion;
}

void menuDificultad(Info * laInfo)
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

void menuCargar(Info * laInfo)
{
	printf("Ingrese el nombre del archivo: ");
	laInfo->nombreArchivoCarga = leerNombreArchivo();
}

char * leerNombreArchivo()
{
	char format[6];
	char * nombreArchivo = malloc(MAX_FILE_NAME_SIZE * sizeof(nombreArchivo[0]));
	if(nombreArchivo == NULL)
		return NULL;
	sprintf(format, "%%%ds", MAX_FILE_NAME_SIZE-1);
	scanf(format, nombreArchivo);
	nombreArchivo = realloc(nombreArchivo, strlen(nombreArchivo)*sizeof(nombreArchivo[0]));
	return nombreArchivo;
}