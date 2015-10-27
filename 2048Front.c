#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "2048Back.h"
#include "getnum.h"

enum {GUARDAR=1, SALIR, _UNDO};

#define CANT_OPCIONES_MENU 3
#define CANT_DIFICULTADES	 3
#define BORRA_BUFFER() while(getchar() != '\n')
#define opcionValida(opcion, cantidad) ((opcion) > 0 && (opcion) <= (cantidad) ? 1 : 0)
#define imprimirData(DIF) printf("Tablero %dx%d\nFicha ganadora: %d\nUndos: %d\n\n", TAMANIO_ ## DIF ,TAMANIO_ ## DIF ,VICTORIA_ ## DIF ,UNDOS_ ## DIF)

void limpiarPantalla();
int menuPrincipal(Info*);
int leerOpcion(int);
void menuDificultad(Info*);
char * menuCargarGuardar(Info*);
char * leerNombreArchivo();
Cod_Error jugar(Info*, Info*);
unsigned short int validarJugadas(Info*);
char leerJugada(const char *, unsigned short int);
void imprimirTablero(const Info *);
void imprimirOpciones();
void imprimirPuntajeyUndos(Info *);

int main(void)
{
	Info infoActual, infoRespaldo;
	Cod_Error hubo_error;
	int opcion;

	opcion = menuPrincipal(&infoActual);

	switch(opcion)
	{
		case SALIR:
				printf("*****¡Adiós!*****\n");
				return 0;
		case JUEGO_NUEVO:
				hubo_error = prepararJuegoNuevo(&infoActual, &infoRespaldo);
				break;
		case CARGAR_JUEGO:
				hubo_error = cargarPartida(&infoActual, &infoRespaldo);
				BORRA_BUFFER();
				break;
	}

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

/*void leerStringSE (char * opcionIngresada)							/*FALTA TERMINAR
{
	char save[]="save", quit[]="quit", undo[]="undo";
	char i, opcion=1, co;
	{
		for (i = 0; opcionIngresada[i] != 0; i++)
		{
			switch (opcion)
			{
				case GUARDAR:

			}
}
*/
void limpiarPantalla()
{
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
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
			laInfo->nombreArchivo = menuCargarGuardar(laInfo);
			break;
		case SALIR:
			break;
	}

	return opcion;
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

void menuDificultad(Info * laInfo)
{
	int opcion;

	limpiarPantalla();
	printf("*****Juego Nuevo*****\n\n");
	printf("1. Fácil\n");
	imprimirData(FACIL);
	printf("2. Medio\n");
	imprimirData(MEDIO);
	printf("3. Difícil\n");
	imprimirData(DIFICIL);

	laInfo->tamanio = dameTamanio(leerOpcion(CANT_DIFICULTADES));
}

char * menuCargarGuardar(Info * laInfo)
{
	printf("Ingrese el nombre del archivo: ");
	return leerNombreArchivo();
}

char * leerNombreArchivo()
{
	char format[6];

	char * nombreArchivo = malloc(MAX_FILE_NAME_SIZE * sizeof(nombreArchivo[0]));

	if(nombreArchivo == NULL)
		return NULL;

	sprintf(format, "%%%ds", MAX_FILE_NAME_SIZE-1);											/* Utilizamos este sprintf para poder poner MAX_FILE_NAME_SIZE, en vez de un magic number. */
	scanf(format, nombreArchivo);

	nombreArchivo = realloc(nombreArchivo, strlen(nombreArchivo)*sizeof(nombreArchivo[0]));

	return nombreArchivo;
}

Cod_Error jugar(Info * laInfoActual, Info * laInfoRespaldo)
{
	unsigned short int cantJugadas;
	char opcionGanar[7];
	char jugada, saleprog=0;
	char c;
	Cod_Error hubo_error;

	do
	{
		limpiarPantalla();
		imprimirPuntajeyUndos(laInfoActual);
		imprimirTablero(laInfoActual);
		cantJugadas = validarJugadas(laInfoActual);
		if (cantJugadas == 0)
		{
			printf("*****PERDISTE*****\n");
			return 0;
		}
		if (laInfoActual->ganaste == FALSE)
		{
			imprimirOpciones();
			jugada = leerJugada(laInfoActual->jugadasValidas, cantJugadas);
			switch(jugada)
			{
				case SALIR:
					do
					{
						printf("¿Desea guardar la partida? Y/N\n");
						if((c = getchar()) == 'Y')
							jugada=GUARDAR;								/*DESP:Buscar otra posible forma de hacerlo*/
						BORRA_BUFFER();
					} while(c != 'N' && c != 'Y');
					saleprog=jugada;
					break;
				case GUARDAR:
					laInfoActual->nombreArchivo = menuCargarGuardar(laInfoActual);
					guardaPartida(laInfoActual);
					BORRA_BUFFER();
					break;
				default:
					hubo_error = actualizarInfo(laInfoActual, laInfoRespaldo, jugada);
					if (hubo_error != OK)
						printf("Error al poner ficha");
		}

	} while(jugada != saleprog);
	}
	/*else
		{
			printf("***** ¡¡¡FELICIDADES, GANASTE!!! *****\n");								/*FALTA TERMINAR
			printf("\n¿Desea salir (quit) o volver al menu principal (volver)?\n");
			scanf("%[a-z], &opcionGanar");
			leerStringSE(const opcionGanar);

		}*/
	return hubo_error;
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
		if (c == QUIT || c == SAVE)
			valida = TRUE;
		else
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
	int i, j, h, recuadro, hayGanadora;
	unsigned short int tamanio;
	tamanio = laInfo->tamanio;
	recuadro = (5 * tamanio) + 1; 						/* Tamanio * 4 es el maximo tamaño que				*/
														/* pueden tener los numeros usados (4 digitos)		*/
	for (h = 0; h < recuadro; h++) 						/* Se le suma tamaño +1 por la separacion entre		*/
		printf("-");									/* los numeros y para tener en cuenta las paredes.	*/
	printf("\n");
	for(i=0; i < tamanio; i++)
		{
			for (j = 0; j < tamanio; j++)
				if (laInfo->tablero[i][j] == 0 )
					printf("|    ");
				else
					printf("|%4d", laInfo->tablero[i][j]);
			printf("|\n");
			for (h = 0; h < recuadro; h++)
				printf("-");
			printf("\n");
		}
}

void imprimirOpciones()
{
	printf("Arriba:\t\t%c\n", ARRIBA);
	printf("Abajo:\t\t%c\n", ABAJO);
	printf("Izquierda:\t%c\n", IZQUIERDA );
	printf("Derecha:\t%c\n", DERECHA );
	printf("Undo:\t\t%s\n", UNDO);
	printf("Guardar:\t%s\n", SAVE);
	printf("Quit:\t\t%s\n", QUIT);
}

void imprimirPuntajeyUndos(Info * laInfoActual)
{
	printf("Puntaje:\t%d\n",laInfoActual->puntaje);
	printf("Undo posible:\t%s\n", laInfoActual->undoPosible ? "Si" : "No");
	printf("Undos:\t\t%d\n", laInfoActual->undos);
}
