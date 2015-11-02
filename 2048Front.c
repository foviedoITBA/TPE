#include <string.h>

#include "2048Back.h"
#include "getnum.h"

#define CANT_OPCIONES_MENU 3
#define CANT_DIFICULTADES 3
#define BORRA_BUFFER() while(getchar() != '\n')
#define opcionValida(opcion, cantidad) ((opcion) > 0 && (opcion) <= (cantidad) ? 1 : 0)
#define imprimirData(DIF) printf("Tablero %dx%d\nFicha ganadora: %d\nUndos: %d\n\n", TAMANIO_ ## DIF ,TAMANIO_ ## DIF ,VICTORIA_ ## DIF ,UNDOS_ ## DIF)

void limpiarPantalla();
int menuPrincipal(Info*);
int leerOpcion(int);
void menuDificultad(Info*);
char * menuCargarGuardar();
char * leerNombreArchivo();
Cod_Error jugar(Info*, Info*);
char leerJugada(Info *, unsigned short int);
void imprimirTablero(const Info *);
void imprimirOpciones();
void imprimirPuntajeyUndos(Info *);

int main(void)
{
	Info infoActual, infoRespaldo;
	Cod_Error hubo_error = OK;
	BOOL juegoTerminado = FALSE;
	int opcion;

	do
	{
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
					hubo_error = cargarJuego(&infoActual, &infoRespaldo);
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
		
		printf("¿Desea volver al menú principal? (s: sí)\n");
		BORRA_BUFFER();
		opcion = getchar();
		BORRA_BUFFER();
		if (opcion != 's')
			juegoTerminado = TRUE;			
	}
	while(juegoTerminado == FALSE);
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
			laInfo->nombreArchivo = menuCargarGuardar();
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
	printf("1: Fácil\n");
	imprimirData(FACIL);
	printf("2: Medio\n");
	imprimirData(MEDIO);
	printf("3: Difícil\n");
	imprimirData(DIFICIL);
	
	laInfo->tamanio = dameTamanio(leerOpcion(CANT_DIFICULTADES));
}

char * menuCargarGuardar()
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
	char jugada, c;
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
		imprimirOpciones();
		jugada = leerJugada(laInfoActual, cantJugadas);
		switch(jugada)
		{
			case QUIT:
				printf("¿Desea guardar el juego? (s: sí)\n");
				c = getchar();
				if (c == 's')
				{
					free(laInfoActual->nombreArchivo);
					laInfoActual->nombreArchivo = menuCargarGuardar();
					guardaPartida(laInfoActual);
					free(laInfoActual->nombreArchivo);
				}
				break;
			case GUARDAR:
				guardaPartida(laInfoActual);
				break;
			default:
				hubo_error = actualizarInfo(laInfoActual, laInfoRespaldo, jugada);
				if (hubo_error != OK)
					printf("Error al poner ficha\n");
		}

	} while(jugada != QUIT && laInfoActual->ganaste != TRUE);

	if (laInfoActual->ganaste == TRUE)
	{
		limpiarPantalla();
		imprimirPuntajeyUndos(laInfoActual);
		imprimirTablero(laInfoActual);
		printf("*****ENHORABUENA, HAS GANADO*****\n");
	}
}

char leerJugada(Info * laInfo, unsigned short int cantJugadas)
{
	char bufferLectura[5 + MAX_FILE_NAME_SIZE]; /* El 5 viene por el "save " */
	BOOL valida = FALSE;
	do
	{
		unsigned short int i;
		fgets(bufferLectura, 30, stdin);
		if (bufferLectura[0] == QUIT)
		{
			if (strcmp(bufferLectura, "quit\n") == 0)
				valida = TRUE;
		}
		else if (bufferLectura[0] == 's' && bufferLectura[1] != '\n') /* Hay que distinguir del abajo */
		{
			if (strncmp(bufferLectura, "save ", 5) == 0 && bufferLectura[5] != '/' && bufferLectura[5] != '\0')
			{
				free(laInfo->nombreArchivo);
				laInfo->nombreArchivo = malloc(strlen(bufferLectura + 5));
				if (laInfo->nombreArchivo != NULL)
				{
					strncpy(laInfo->nombreArchivo, bufferLectura + 5, strlen(bufferLectura + 5) - 1); /* Que no ponga el '\n' */
					bufferLectura[0] = GUARDAR; /* Para que el menú distinga entre guardar y abajo */
					valida = TRUE;
				}
			}
		}
		else if (bufferLectura[0] == UNDO)
		{
			if (strcmp(bufferLectura, "undo\n") == 0)
				for (i = 0; i < cantJugadas; i++)
					if (laInfo->jugadasValidas[i] == UNDO)
						valida = TRUE;
		}
		else
			for (i = 0; i < cantJugadas && !valida; i++)
				if (bufferLectura[0] == laInfo->jugadasValidas[i] && bufferLectura[1] == '\n')
					valida = TRUE;
		if (valida == FALSE)
			printf("JUGADA INVÁLIDA\n");
	} while (valida == FALSE);
	return bufferLectura[0];
}

void imprimirTablero(const Info * laInfo)
{
	int i, j, h, recuadro;
	unsigned short int tamanio;
	tamanio = laInfo->tamanio;
	recuadro = (4 * tamanio) + tamanio + 1; 						/*(pongo el tamanio+1 porque son los * que separan a los numeros en cada fila) */

	for (h = 0; h < recuadro; h++) 									/*se que queda muy villero poner las lineas horizontales que separan las filas asi pero googlie y no hay otra forma	*/
		printf("-");												/*para hacerlo, solo se puede hacer con loops porque no hay otra forma de hacer un "imprimime x numero de veces un	*/
	printf("\n");													/*caracter"																											*/
	for(i=0; i < tamanio; i++)										/*El for que hace printf * podria ser una macro, right?*/
		{
			for (j = 0; j < tamanio; j++)
				if (laInfo->tablero[i][j] ==0 )
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
	printf("Undo:\t\tundo\n");
	printf("Guardar:\tsave <nombre del archivo>\n");
	printf("Quit:\t\tquit\n\n");
}

void imprimirPuntajeyUndos(Info * laInfoActual)
{
	printf("Puntaje:\t%d\n",laInfoActual->puntaje);
	printf("Undo posible:\t%s\n", laInfoActual->undoPosible ? "Si" : "No");
	printf("Undos:\t\t%d\n", laInfoActual->undos);
}

