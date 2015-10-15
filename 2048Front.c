#include <stdio.h>
#include <stdlib.h>

#include "2048Tipos.h"
#include "2048Back.h"

#define BORRA_BUFFER() (while(getchar() != '\n'))

int main(void)
{
	INFO infoActual, infoRespaldo;
	
