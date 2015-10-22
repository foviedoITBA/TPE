#include "2048Back.h"
#include <stdio.h> 			/* BORRAR DESPUES */

static Cod_Error inicializarNuevo(Info *);
static Tablero crearTablero(unsigned short int);
static void liberarTablero(Info*);
static void fichaAlAzar(Ficha*, unsigned short int*, unsigned short int*, Tamanio);
static void copiarInfo(Info*, const Info*);
static void mover(Info*, char);
static Cod_Error ponerFicha(Info *, char);
static int recorrerTablero(const Info *, char, posicionLibre *);
static int recorrerTableroIzquierdaoDerecha(const Info *, char, posicionLibre *);
static int recorrerTableroArribaoAbajo(const Info *, char, posicionLibre *);
static void mover(Info*,char);


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

Cod_Error prepararJuegoNuevo(Info * laInfoActual, Info * laInfoRespaldo)			/* Deja el tablero principal con dos fichas y todos ceros. */
{
	int result;

	unsigned short int x1, y1, x2, y2;										/* Coordenadas de las 2 fichas a agregar. */
	Ficha laFicha;
	laInfoRespaldo->tamanio = laInfoActual->tamanio;						/* Le asigna al estado de respaldo el mismo tamanio para poder inicializarlo */

	result = inicializarNuevo(laInfoActual);

	if (result == ERROR_MEMORIA)
		return ERROR_MEMORIA;

	result = inicializarNuevo(laInfoRespaldo);

	if (result == ERROR_MEMORIA)
	{
		liberarTablero(laInfoActual);
		return ERROR_MEMORIA;
	}
	

	fichaAlAzar(&laFicha, &x1, &y1, laInfoActual->tamanio);					/* Ponemos la primera ficha. */
	laInfoActual->tablero[x1][y1] = laFicha;
	do
		fichaAlAzar(&laFicha, &x2, &y2, laInfoActual->tamanio);				/* Ponemos la segunda ficha. */
	while(x2 == x1 && y2 == y1);

	laInfoActual->tablero[x2][y2] = laFicha;
	return result;
}

Cod_Error cargarJuego(Info * laInfoActual, Info * laInfoRespaldo)
{
	laInfoActual->tablero = crearTablero(laInfoActual->tamanio);

	if(laInfoActual->tablero == NULL)
		return ERROR_MEMORIA;
		
	laInfoRespaldo->tablero = crearTablero(laInfoRespaldo->tamanio);

	if(laInfoRespaldo->tablero == NULL)
	{
		liberarTablero(laInfoActual);
		return ERROR_MEMORIA;
	}

	/* parte de cargado*/
	return OK;	/*cambiar dsp */
}


static Cod_Error inicializarNuevo(Info * laInfo)
{
	int i, j;
	unsigned short int dificultad = dameDificultad(laInfo->tamanio);

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

	laInfo->tablero = crearTablero(laInfo->tamanio);
	if (laInfo->tablero == NULL)
		return ERROR_MEMORIA;
	
	for (i = 0; i < laInfo->tamanio; i++)
		for (j = 0; j < laInfo->tamanio; j++)
			laInfo->tablero[i][j] = 0;

	laInfo->undoPosible = FALSE;
	laInfo->nombreArchivoCarga = NULL;

	return OK;
}

static Tablero crearTablero(unsigned short int tamanio)
{
	unsigned short int i;
	Tablero tablero;
	
	tablero = malloc(tamanio * sizeof(tablero[0]));

	if (tablero == NULL)
		return NULL;
	
	for (i = 0; i < tamanio; i++)
	{
		tablero[i] = malloc(tamanio * sizeof(tablero[0][0]));
		
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
		copiarInfo(laInfoActual, laInfoRespaldo);
		laInfoActual->undos -= 1;
		laInfoActual->undoPosible = FALSE;
	}
	else
	{
		copiarInfo(laInfoRespaldo, laInfoActual);
		mover(laInfoActual, jugada);
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

static void mover(Info * laInfo, char direccion)
{	/* Falta hacerla */
	return;
}

static Cod_Error ponerFicha(Info * laInfo, char ultimaDireccion)
{
	int auxRand;
	unsigned short int cantLibre;
	posicionLibre * posiciones;
	posiciones = malloc((laInfo->tamanio * laInfo->tamanio)*sizeof(posicionLibre));

	if(posiciones == NULL)
		return ERROR_MEMORIA;

	cantLibre=recorrerTablero(laInfo, ultimaDireccion, posiciones);

	auxRand = rand()%cantLibre;
	
	laInfo->tablero[posiciones[auxRand].x][posiciones[auxRand].y] = FICHA_NUEVA();
}

unsigned short int validarJugadas(Info * laInfo)
{	/* Falta hacerla */
	if(laInfo->undoPosible && laInfo->undos > 0)
		laInfo->jugadasValidas[0] = 'u';
	else
		laInfo->jugadasValidas[0] = 0;
	
	return 2;
}

static int recorrerTablero(const Info * laInfo, char direccion, posicionLibre * posiciones)		/* Se requieren dos funciones: 			*/
{																								/* recorrerTableroIzquierdaoDerecha y	*/
	if (direccion == ARRIBA || direccion == ABAJO)												/* recorrerTableroArribaoAbajo,	pues	*/
		return recorrerTableroArribaoAbajo(laInfo, direccion, posiciones);						/* de esta manera se puede optimizar	*/
	if (direccion == IZQUIERDA || direccion == DERECHA)											/* la busqueda de posiciones vacias,	*/
		return recorrerTableroIzquierdaoDerecha(laInfo, direccion, posiciones);					/* cambiando el sentido de recorrido:	*/				
}																								/* En un caso por filas y luego columnas*/
																								/* Y en el otro, al revez. */


static int recorrerTableroIzquierdaoDerecha(const Info * laInfo, char direccion, posicionLibre * posiciones)
{
	int i,j, cant = 0;
	int libre;
	unsigned short int posicionInicialFil, posicionInicialCol;
	unsigned short int incrementoFil, incrementoCol;

	switch(direccion)
	{
		case IZQUIERDA:
			posicionInicialFil = 0;
			posicionInicialCol = laInfo->tamanio - 1;
			incrementoFil = +1;
			incrementoCol = -1;
			break;
		case DERECHA:
			posicionInicialFil = 0;
			posicionInicialCol = 0;
			incrementoFil = +1;
			incrementoCol = +1;
			break;
	}

	for( i = posicionInicialFil; i <= laInfo->tamanio - 1 ; i += incrementoFil)
	{
		libre = 1;
		for(j = posicionInicialCol; 0<= j && j <= laInfo->tamanio - 1 && libre ; j += incrementoCol)
		{
			if(laInfo->tablero[i][j] != 0)
				libre = 0;
			posiciones[cant].x = i;
			posiciones[cant].y = j;
			cant++;
		}
	}

	return cant;
}


static int recorrerTableroArribaoAbajo(const Info * laInfo, char direccion, posicionLibre * posiciones)
{
	int i,j, cant = 0;
	int libre;
	unsigned short int posicionInicialFil, posicionInicialCol;
	unsigned short int incrementoFil, incrementoCol;

	switch(direccion)
	{
		case ARRIBA:
			posicionInicialFil = laInfo->tamanio - 1;
			posicionInicialCol = 0;
			incrementoFil = -1;
			incrementoCol = +1;
			break;
		case ABAJO:
			posicionInicialFil = 0;
			posicionInicialCol = 0;
			incrementoFil = +1;
			incrementoCol = +1;
			break;
	}
	for(j = posicionInicialCol; j <= laInfo->tamanio - 1 && libre ; j += incrementoCol)
	{
		libre = 1;
		for( i = posicionInicialFil; 0 <= i && i <= laInfo->tamanio - 1 ; i += incrementoFil)
		{
			if(laInfo->tablero[i][j] != 0)
				libre = 0;

			posiciones[cant].x = i;
			posiciones[cant].y = j;
			cant++;
		}
	}
	
	return cant;
}