#include "2048Back.h"

unsigned short int dameTamanio(Dificultad dif)
{
	switch(dif)
	{
		case DIF_FACIL:
			return TAMANIO_FACIL;
		case DIF_MEDIO:
			return TAMANIO_MEDIO;
		case DIF_DIFICIL:
			return TAMANIO_DIFICIL;
		default:
			return 0;
	}
}

Cod_Error prepararJuego(Info * laInfoActual, Info * laInfoRespaldo, int opcion)
{
	int result;
	if (opcion == JUEGO_NUEVO)
	{
		result = inicializarNuevo(laInfoActual);
		if (result == ERROR_MEMORIA)
			return ERROR_MEMORIA;
		result = inicializarNuevo(laInfoRespaldo);
		if (result == ERROR_MEMORIA)
			liberarTablero(laInfoActual);
		return result;
	}
	else if (opcion == CARGAR_JUEGO)
	{
		/*cargar juego*/
	}
}

static Cod_Error inicializarNuevo(Info * laInfo)
{
	int i, j;
	unsigned short int tamanio = dameTamanio(laInfo->dificultad);

	laInfo->puntaje = 0;
	switch(laInfo->dificultad)
	{
		case DIF_FACIL:
			laInfo->undos = UNDOS_FACIL;
			break;
		case DIF_MEDIO:
			laInfo->undos = UNDOS_MEDIO;
			break;
		case DIF_DIFICIL:
			laInfo->undos = UNDOS_DIFICIL;
			break;
	}

	laInfo->tablero = crearTablero(tamanio);
	if (laInfo->tablero == NULL)
		return ERROR_MEMORIA;
	
	for (i = 0; i < tamanio; i++)
		for (j = 0; j < tamanio; j++)
			laInfo->tablero[i][j] = 0;

	laInfo->nombreArchivoCarga = NULL;

	return OK;
}

static Tablero crearTablero(unsigned short int tamanio)
{
	unsigned short int i;
	Tablero tablero;
	
	tablero = malloc(tamanio*sizeof(tablero[0]));
	if (tablero == NULL)
		return NULL;
	for (i = 0; i < tamanio; i++)
	{
		tablero[i] = malloc(tamanio*sizeof(tablero[0][0]));
		if (tablero[i] == NULL)
		{
			for (; i >= 0; i--)
				free(tablero[i]);
			free(tablero);
			tablero = NULL;
			return NULL;
		}
	}

	return tablero;
}

static void liberarTablero(Info * laInfo)
{
	unsigned short int i;
	unsigned short int tamanio = dameTamanio(laInfo->dificultad);
	for (i = 0; i < tamanio; i++)
		free(laInfo->tablero[i]);
	free(laInfo->tablero);
	laInfo->tablero = NULL;
}


