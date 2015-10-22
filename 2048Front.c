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
Cod_Error jugar(Info*, Info*);
unsigned short int validarJugadas(Info*);
char leerJugada(const char *, unsigned short int);
void imprimirTablero(const Info *);
void imprimirOpciones();

void verQueOnda(Info * laInfo)
{
	int i, j;
	unsigned short int tamanio;
	limpiarPantalla();
	printf("\n");
	for (i = 0; i < laInfo->tamanio; i++)
	{
		for (j = 0; j < laInfo->tamanio; j++)
			printf("%hu", laInfo->tablero[i][j]);
		printf("\n");
	}
	printf("\n\n");

	printf("Undos: %hu\nPuntaje: %u\n", laInfo->undos, laInfo->puntaje);
}

int main(void)
{
	Info infoActual, infoRespaldo;
	Cod_Error hubo_error;
	int opcion = menuPrincipal(&infoActual);
	if (opcion == SALIR)
	{
		printf("*****¡Adiós!*****\n");
		return 0;
	}
	srand(time(0));
	hubo_error = prepararJuego(&infoActual, &infoRespaldo, opcion);
	if (hubo_error == ERROR_MEMORIA)
	{
		printf("ERROR: FALTA MEMORIA\n");
		return hubo_error;
	}
	else if (hubo_error == ERROR_ARCHIVO)
	{
		printf("ERROR: ARCHIVO INVÁLIDO\n");
		return hubo_error;
	}

	hubo_error = jugar(&infoActual, &infoRespaldo);

	return hubo_error;
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
	
	laInfo->tamanio = dameTamanio(leerOpcion(CANT_DIFICULTADES));
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

Cod_Error jugar(Info * laInfoActual, Info * laInfoRespaldo)
{
	unsigned short int cantJugadas;
	char jugada;
	do
	{
		limpiarPantalla();
		imprimirTablero(laInfoActual);
		cantJugadas = validarJugadas(laInfoActual);
		if (cantJugadas == 0)
		{
			printf("*****PERDISTE*****\n");
			return 0;
		}
		imprimirOpciones();
		jugada = leerJugada(laInfoActual->jugadasValidas, cantJugadas);
		switch(jugada)
		{
			case QUIT:
				/* Algo para guardar y salir */
				break;
			case GUARDAR:
				/* Algo para guardar */
				break;
			default:
				actualizarInfo(laInfoActual, laInfoRespaldo, jugada);
		}

	} while(jugada != QUIT);
}

char leerJugada(const char * jugadasValidas, unsigned short int cantJugadas)
{
	int c;
	BOOL valida = FALSE;
	do
	{
		unsigned short int i;
		c = getchar();
		BORRA_BUFFER();
		if (c == QUIT || c == GUARDAR)
			valida = TRUE;
		for (i = 0; i < cantJugadas && !valida; i++)
			if (c == jugadasValidas[i])
				valida = TRUE;
		if (valida == FALSE)
			printf("JUGADA INVÁLIDA\n");
	} while (valida == FALSE);
	return (char) c;
}

void imprimirTablero(const Info * laInfo)
{
	int i, j, h, recuadro;
	unsigned short int tamanio;
	tamanio = laInfo->tamanio;
	recuadro = (4*tamanio)+tamanio+1; 				/*(pongo el tamanio+1 porque son los * que separan a los numeros en cada fila)*/
	for (h=0; h<recuadro; h++) 						/*se que queda muy villero poner las lineas horizontales que separan las filas asi pero googlie y no hay otra forma*/
		printf("*");								/*para hacerlo, solo se puede hacer con loops porque no hay otra forma de hacer un "imprimime x numero de veces un*/
	printf("\n");									/*caracter"*/
	for(i=0; i < tamanio; i++)						
		{
			for (j=0; j < tamanio; j++)
					printf("*%4d", laInfo->tablero[i][j]);
			printf("*\n");
			for (h=0; h<recuadro; h++)
				printf("*");
			printf("\n");
		}
}

void imprimirOpciones()
{
	printf("Arriba: %c\tAbajo: %c\tIzquierda: %c\tDerecha: %c\tUndo: %c\tGuardar: %c\tSalir: %c\t\n", ARRIBA, ABAJO, IZQUIERDA, DERECHA, UNDO, GUARDAR, QUIT);
}