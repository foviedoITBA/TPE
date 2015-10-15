#ifndef BACK_H
#define BACK_H

#include <stdlib.h>
#include <time.h>
#include "2048Tipos.h"

#define FICHA_NUEVA() ((rand() / (double) RAND_MAX) <= 0.89 ? 2 : 4)

#define UNDOS_FACIL 8
#define UNDOS_MEDIO 4
#define UNDOS_DIFICIL 2

#define TAMANIO_FACIL 8
#define TAMANIO_MEDIO 6
#define TAMANIO_DIFICIL 4

#define VICTORIA_FACIL 1024
#define VICTORIA_MEDIO 2048
#define VICTORIA_DIFICIL 2048

#endif
