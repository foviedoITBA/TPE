FILE * guardaPartida(Info * laInfo)
{
	int i,j;
	unsigned short int dif = dameDificultad(laInfo->tamanio);
	FILE *archivoGuarda;
	archivoGuarda = fopen(laInfo->nombreArchivoCarga, "wb");
	
	fwrite(&dif, sizeof(Dificultad),1,archivoGuarda);
	fwrite(&laInfo->puntaje, sizeof(Puntaje),1,archivoGuarda);
	
	for(i = 0; i < laInfo->tamanio ; i++)
		fwrite(laInfo->tablero[i] ,sizeof(laInfo->tablero[0][0]), tam, archivoGuarda);

	return archivoGuarda;
}




