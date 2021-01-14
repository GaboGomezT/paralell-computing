#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#include "imagen.h"
#include "helper.h"
#include "procesamiento.h"
#include "defs.h"
#include "hilos.h"

unsigned char *imagenRGB, *imagenGray, *imagenSobel;
bmpInfoHeader info;

int main( )
{

	int *hilo;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	register int nh;

	imagenRGB = abrirBMP("calle1.bmp", &info );

	displayInfo( &info );
	imagenGray = reservarMemoria( info.width, info.height );
	imagenSobel = reservarMemoria( info.width, info.height );
	memset( imagenSobel, 255, info.width*info.height );

	RGBToGray( imagenRGB, imagenGray, info.width, info.height );

	printf("Creando hilos...\n");
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funHiloSobel, &nhs[nh]);
	}

	printf("Esperando hilos...\n");
	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&hilo);
		printf("El hilo %d termino \n", *hilo);
	}

	GrayToRGB( imagenRGB, imagenSobel, info.width, info.height );

	guardarBMP("calle1Sobel.bmp", &info, imagenRGB );

	free( imagenRGB );
	free( imagenGray );
	free( imagenSobel );

	return 0;
}