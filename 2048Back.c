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
static void moverIzquierdaDerecha(Info*,char);
static void moverArribaAbajo(Info*,char);
static BOOL validarArriba(const Info*, unsigned short int, unsigned short int);
static BOOL validarAbajo(const Info*, unsigned short int, unsigned short int);
static BOOL validarIzquierda(const Info*, unsigned short int, unsigned short int);
static BOOL validarDerecha(const Info*, unsigned short int, unsigned short int);

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

unsigned short int dameVictoria(unsigned short int dif)
{
	switch(dif)
	{
		case DIF_FACIL:
			return VICTORIA_FACIL;
		case DIF_MEDIO:
			return VICTORIA_MEDIO;
		case DIF_DIFICIL:
			return VICTORIA_DIFICIL;
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
	
	randomizeSeed();
	/* Ponemos la primera ficha. */
	fichaAlAzar(&laFicha, &x1, &y1, laInfoActual->tamanio);					
	laInfoActual->tablero[x1][y1] = laFicha;
	
	/* Ponemos la segunda ficha. */
	do
		fichaAlAzar(&laFicha, &x2, &y2, laInfoActual->tamanio);
	while(x2 == x1 && y2 == y1);

	laInfoActual->tablero[x2][y2] = laFicha;
	return result;
}

Cod_Error cargarJuego(Info * laInfoActual, Info * laInfoRespaldo)
{
	FILE * archivoCarga;
	unsigned short int dif;
	int i,j;
	randomizeSeed();
	
	laInfoActual->undoPosible = FALSE;

	archivoCarga = fopen(laInfoActual->nombreArchivo, "rb");
	
	

	fread(&dif, sizeof(dif), 1, archivoCarga);
	
	laInfoActual->tamanio = dameTamanio(dif);
	laInfoRespaldo->tamanio = laInfoActual->tamanio;
	laInfoActual->undoPosible = FALSE;
	laInfoActual->undoPosible = FALSE;

	laInfoActual->tablero = crearTablero(laInfoActual->tamanio);
	if(laInfoActual->tablero == NULL)
		return ERROR_MEMORIA;
	
	laInfoRespaldo->tablero = crearTablero(laInfoRespaldo->tamanio);
	if(laInfoRespaldo->tablero == NULL)
	{
		liberarTablero(laInfoActual);
		return ERROR_MEMORIA;
	}
	
	fread(&(laInfoActual->puntaje), sizeof(Puntaje), 1, archivoCarga);
	fread(&(laInfoActual->undos), sizeof(unsigned short int), 1, archivoCarga);
	for(i = 0; i < laInfoActual->tamanio; i++)
		fread(laInfoActual->tablero[i], sizeof(Ficha), laInfoActual->tamanio, archivoCarga);

	fclose(archivoCarga);
	return OK;
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
	laInfo->nombreArchivo = NULL;
	laInfo->ganaste = FALSE;

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

Cod_Error actualizarInfo(Info * laInfoActual, Info * laInfoRespaldo, char jugada)
{
	if (jugada == UNDO)
	{
		copiarInfo(laInfoActual, laInfoRespaldo);
		laInfoActual->undos -= 1;
		laInfoActual->undoPosible = FALSE;
		return OK;
	}
	else
	{
		Cod_Error hubo_error;
		copiarInfo(laInfoRespaldo, laInfoActual);
		mover(laInfoActual, jugada);
		hubo_error = ponerFicha(laInfoActual, jugada);
		laInfoActual->undoPosible = TRUE;
		return hubo_error;
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

static void mover(Info * laInfo, char jugada)
{
	if (jugada == IZQUIERDA || jugada == DERECHA)
		moverIzquierdaDerecha(laInfo, jugada);
	else
		moverArribaAbajo(laInfo, jugada);
}

static void moverIzquierdaDerecha(Info * laInfo, char direccion)
{	
	int incJ, incK;
	Estado estado;
	int index, i, j;
	Ficha ficha;
	if (direccion == DERECHA)
	{
		incJ = -1;
		incK = 1;
	}
	else if (direccion == IZQUIERDA)
	{
		incJ = 1;
		incK = -1;
	}
	else
		return;
	for (i = 0; i < laInfo->tamanio; i++)
	{
		estado = NADA;
		if (direccion == DERECHA)
			j = laInfo->tamanio-1;
		else if (direccion == IZQUIERDA)
			j = 0;
		for (; j >= 0 && j < laInfo->tamanio; j += incJ)
		{
			switch(estado)
			{
				case NADA:
					if (laInfo->tablero[i][j] != 0)
					{
						signed short int k;
						ficha = laInfo->tablero[i][j];
						for (k = j; ((k < laInfo->tamanio - 1 && k > 0) || (k + incK < laInfo->tamanio - 1 && k + incK > 0)) && laInfo->tablero[i][k+incK] == 0; k += incK);
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[i][k] = ficha;
						index = k;
						estado = NUMERO;
					}
					break;
				case NUMERO:
					if (laInfo->tablero[i][j] == ficha)
					{
						laInfo->puntaje += 2*ficha;
						if (2*ficha == dameVictoria(dameDificultad(laInfo->tamanio)))
							laInfo->ganaste = TRUE;
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[i][index] = 2*ficha;
						estado = NADA;
					}
					else if (laInfo->tablero[i][j] != 0)
					{
						signed short int k;
						ficha = laInfo->tablero[i][j];
						for (k = j; ((k < laInfo->tamanio - 1 && k > 0) || (k + incK < laInfo->tamanio - 1 && k + incK > 0)) && laInfo->tablero[i][k+incK] == 0; k+=incK);
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[i][k] = ficha;
						index = k;
					}
					break;
			}
		}
	}
}

static void moverArribaAbajo(Info * laInfo, char direccion)
{
	int incI, incK;
	Estado estado;
	int index, i, j;
	Ficha ficha;
	if (direccion == ABAJO)
	{
		incI = -1;
		incK = 1;
	}
	else if (direccion == ARRIBA)
	{
		incI = 1;
		incK = -1;
	}
	else
		return;
	for (j = 0; j < laInfo->tamanio; j++)
	{
		estado = NADA;
		if (direccion == ABAJO)
			i = laInfo->tamanio-1;
		else if (direccion == ARRIBA)
			i = 0;
		for (; i >= 0 && i < laInfo->tamanio; i += incI)
		{
			switch(estado)
			{
				case NADA:
					if (laInfo->tablero[i][j] != 0)
					{
						signed short int k = 0;
						ficha = laInfo->tablero[i][j];
						for (k = i; ((k < laInfo->tamanio - 1 && k > 0) || (k + incK < laInfo->tamanio - 1 && k + incK > 0)) && laInfo->tablero[k+incK][j] == 0; k += incK);
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[k][j] = ficha;
						index = k;
						estado = NUMERO;
					}
					break;
				case NUMERO:
					if (laInfo->tablero[i][j] == ficha)
					{
						laInfo->puntaje += 2*ficha;
						if (2*ficha == dameVictoria(dameDificultad(laInfo->tamanio)))
							laInfo->ganaste = TRUE;
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[index][j] = 2*ficha;
						estado = NADA;
					}
					else if (laInfo->tablero[i][j] != 0)
					{
						signed int k;
						ficha = laInfo->tablero[i][j];
						for (k = i; ((k < laInfo->tamanio - 1 && k > 0) || (k + incK < laInfo->tamanio - 1 && k + incK > 0)) && laInfo->tablero[k+incK][j] == 0; k+=incK);
						laInfo->tablero[i][j] = 0;
						laInfo->tablero[k][j] = ficha;
						index = k;
					}
					break;
			}
		}
	}
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

	free(posiciones);
	return OK;
}

unsigned short int validarJugadas(Info * laInfo)
{	/* Falta hacerla */
	BOOL arriba_valida = FALSE, abajo_valida = FALSE, izquierda_valida = FALSE, derecha_valida = FALSE;
	unsigned short int i, j, cantJugadas = 0;
	for (i = 0; i < laInfo->tamanio && cantJugadas < 4; i++)
		for (j = 0; j < laInfo->tamanio && cantJugadas < 4; j++)
		{
			if (arriba_valida == FALSE)
				arriba_valida = validarArriba(laInfo, i, j);
			if (abajo_valida == FALSE)
				abajo_valida = validarAbajo(laInfo, i, j);
			if (izquierda_valida == FALSE)
				izquierda_valida = validarIzquierda(laInfo, i, j);
			if (derecha_valida == FALSE)
				derecha_valida = validarDerecha(laInfo, i, j);
		}

	if (arriba_valida == TRUE)
		laInfo->jugadasValidas[cantJugadas++] = ARRIBA;
	if (abajo_valida == TRUE)
		laInfo->jugadasValidas[cantJugadas++] = ABAJO;
	if (izquierda_valida == TRUE)
		laInfo->jugadasValidas[cantJugadas++] = IZQUIERDA;
	if (derecha_valida == TRUE)
		laInfo->jugadasValidas[cantJugadas++] = DERECHA;

	if (laInfo->undoPosible == TRUE && laInfo->undos > 0)
		laInfo->jugadasValidas[cantJugadas++] = UNDO;

	return cantJugadas;
}

static BOOL validarArriba(const Info * laInfo, unsigned short int i, unsigned short int j)
{
	if (laInfo->tablero[i][j] == 0)
		return FALSE;
	if (i == 0)
		return FALSE;
	if (laInfo->tablero[i][j] == laInfo->tablero[i-1][j])
		return TRUE;
	if (laInfo->tablero[i-1][j] == 0)
		return TRUE;
	return FALSE;
}

static BOOL validarAbajo(const Info * laInfo, unsigned short int i, unsigned short int j)
{
	if (laInfo->tablero[i][j] == 0)
		return FALSE;
	if (i == laInfo->tamanio - 1)
		return FALSE;
	if (laInfo->tablero[i][j] == laInfo->tablero[i+1][j])
		return TRUE;
	if (laInfo->tablero[i+1][j] == 0)
		return TRUE;
	return FALSE;
}

static BOOL validarIzquierda(const Info * laInfo, unsigned short int i, unsigned short int j)
{
	if (laInfo->tablero[i][j] == 0)
		return FALSE;
	if (j == 0)
		return FALSE;
	if (laInfo->tablero[i][j] == laInfo->tablero[i][j-1])
		return TRUE;
	if (laInfo->tablero[i][j-1] == 0)
		return TRUE;
	return FALSE;
}

static BOOL validarDerecha(const Info * laInfo, unsigned short int i, unsigned short int j)
{
	if (laInfo->tablero[i][j] == 0)
		return FALSE;
	if (j == laInfo->tamanio - 1)
		return FALSE;
	if (laInfo->tablero[i][j] == laInfo->tablero[i][j+1])
		return TRUE;
	if (laInfo->tablero[i][j+1] == 0)
		return TRUE;
	return FALSE;
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
	BOOL libre;
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
		libre = TRUE;
		for(j = posicionInicialCol; 0<= j && j <= laInfo->tamanio - 1 && libre == TRUE ; j += incrementoCol)
		{
			if(laInfo->tablero[i][j] != 0)
				libre = FALSE;
			else
			{
				posiciones[cant].x = i;
				posiciones[cant].y = j;
				cant++;
			}
		}
	}

	return cant;
}


static int recorrerTableroArribaoAbajo(const Info * laInfo, char direccion, posicionLibre * posiciones)
{
	int i,j, cant = 0;
	BOOL libre;
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
	for(j = posicionInicialCol; j <= laInfo->tamanio - 1 ; j += incrementoCol)
	{
		libre = TRUE;
		for( i = posicionInicialFil; 0 <= i && i <= laInfo->tamanio - 1 && libre == TRUE ; i += incrementoFil)
		{
			if(laInfo->tablero[i][j] != 0)
				libre = FALSE;
			else
			{
				posiciones[cant].x = i;
				posiciones[cant].y = j;
				cant++;
			}
		}
	}
	
	return cant;
}


void guardaPartida(Info * laInfo)
{
	int i,j;
	unsigned short int dif = dameDificultad(laInfo->tamanio);
	FILE * archivoGuarda;

	archivoGuarda = fopen(laInfo->nombreArchivo, "wb");
	
	fwrite(&dif, sizeof(unsigned short int), 1, archivoGuarda);
	fwrite(&(laInfo->puntaje), sizeof(Puntaje), 1, archivoGuarda);	
	fwrite(&(laInfo->undos), sizeof(unsigned short int), 1, archivoGuarda);
	for(i = 0; i < laInfo->tamanio ; i++)
		fwrite(laInfo->tablero[i], sizeof(laInfo->tablero[0][0]), laInfo->tamanio, archivoGuarda);

	fclose(archivoGuarda);
}