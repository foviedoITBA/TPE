#include "2048Back.h"

static Cod_Error inicializarNuevo(Info *);
static Tablero crearTablero(unsigned short int);
static void liberarTablero(Info*);
static void fichaAlAzar(Ficha*, unsigned short int*, unsigned short int*, Tamanio);
static void copiarInfo(Info*, Info*);
static void mover(Info*, char);

static void fichaAlAzar(Ficha * laFicha, unsigned short int * x, unsigned short int * y, Tamanio tam)
{
	*x = rand()%tam;
	*y = rand()%tam;
	*laFicha = FICHA_NUEVA();
}

Tamanio dameTamanio(unsigned int dif)
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

unsigned short int dameDificultad(Tamanio tam)
{
	switch(tam)
	{
		case TAMANIO_FACIL:
			return DIF_FACIL;
		case TAMANIO_MEDIO:
			return DIF_MEDIO;
		case TAMANIO_DIFICIL:
			return DIF_DIFICIL;
		default:
			return 0;
	}
}

Cod_Error prepararJuego(Info * laInfoActual, Info * laInfoRespaldo, int opcion)
{
	int result;
	if (opcion == JUEGO_NUEVO)
	{
		unsigned short int x1, y1, x2, y2;
		Ficha laFicha;
		result = inicializarNuevo(laInfoActual);
		if (result == ERROR_MEMORIA)
			return ERROR_MEMORIA;
		result = inicializarNuevo(laInfoRespaldo);
		if (result == ERROR_MEMORIA)
		{
			liberarTablero(laInfoActual);
			return ERROR_MEMORIA;
		}
		
		// Ponemos la primera ficha 
		fichaAlAzar(&ficha, &x1, &y1, laInfo->tamanio);
		laInfoActual->tablero[x1][y1] = ficha;
		
		// Ponemos la segunda ficha
		do
			fichaAlAzar(&laFicha, &x2, &y2);
		while(x2 == x1 && y2 == y1);
		laInfoActual->tablero[x2][y2] = ficha;

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
	unsigned short int dificultad = dameDificultad(laInfo);

	laInfo->puntaje = 0;
	switch(dificultad)
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

	laInfo->undoPosible = FALSE;
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
	for (i = 0; i < laInfo->tamanio; i++)
		free(laInfo->tablero[i]);
	free(laInfo->tablero);
	laInfo->tablero = NULL;
}

void actualizarInfo(Info * laInfoActual, Info * laInfoRespaldo, char jugada)
{
	if (jugada == UNDO)
	{
		copiarInfo(&laInfoActual, &laInfoRespaldo);
		laInfoActual->undos -= 1;
		laInfoActual->undoPosible = FALSE;
	}
	else
	{
		copiarInfo(&laInfoRespaldo, &laInfoActual);
		mover(&laInfoActual, jugada);
		laInfoActual->undoPosible = TRUE;
	}
}

static void copiarInfo(Info * infoDestino, const Info * infoFuente)
{
	unsigned short int i, j;
	infoDestino->puntaje = infoFuente->puntaje;
	for (i = 0; i < infoFuente->tamanio; i++)
		for (j = 0; j < infoFuente->tamanio; j++)
			infoDestino->tablero[i][j] = infoFuente->tablero[i][j];
}