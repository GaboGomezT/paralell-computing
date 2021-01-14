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
#define DIMASK 3

void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height );
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height );
void Sobel( unsigned char *imagenG, unsigned char *imagenS, uint32_t width, uint32_t height );


int main( )
{
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray, *imagenFiltrada;

	imagenRGB = abrirBMP("calle1.bmp", &info );

	displayInfo( &info );
	imagenGray = reservarMemoria( info.width, info.height );
	imagenFiltrada = reservarMemoria( info.width, info.height );
	memset( imagenFiltrada, 255, info.width*info.height );

	RGBToGray( imagenRGB, imagenGray, info.width, info.height );

	Sobel( imagenGray, imagenFiltrada, info.width, info.height );

	GrayToRGB( imagenRGB, imagenFiltrada, info.width, info.height );

	guardarBMP("calle1Sobel.bmp", &info, imagenRGB );

	free( imagenRGB );
	free( imagenGray );
	free( imagenFiltrada );

	return 0;
}

void Sobel( unsigned char *imagenG, unsigned char *imagenS, uint32_t width, uint32_t height )
{
	register int x, y, xm, ym;
	int indicem, indicei, convFila, convCol, factor = 4;
	int gradienteFila[DIMASK*DIMASK] = {
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};
	int gradienteColumna[DIMASK*DIMASK] = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};
	for( y = 0; y <= height-DIMASK; y++ )
		for( x = 0; x <= width-DIMASK; x++ )
		{
			indicem = 0;
			convFila = 0;
			convCol = 0;
			for( ym = 0; ym < DIMASK; ym++ )
				for( xm = 0; xm < DIMASK; xm++ )
				{
					indicei = (y+ym)*width + (x+xm);
					convFila += imagenG[indicei] * gradienteFila[indicem];
					convCol += imagenG[indicei] * gradienteColumna[indicem++];
				}
			convFila = convFila / factor;
			convCol = convCol / factor;
			indicei = (y+1)*width + (x+1);
			imagenS[indicei] = (unsigned char)sqrt((convFila * convFila) + (convCol * convCol));
		}
}

void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	int indiceRGB, indiceGray;

	for( indiceGray = 0, indiceRGB = 0; indiceGray < (height*width); indiceGray++, indiceRGB += 3 )
	{
		imagenRGB[indiceRGB]   = imagenGray[indiceGray];
		imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
		imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
	}
}
void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	unsigned char nivelGris;
	int indiceRGB, indiceGray;

	for( indiceGray = 0, indiceRGB = 0; indiceGray < (height*width); indiceGray++, indiceRGB += 3 )
	{
		nivelGris = (30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2]) / 100;
		imagenGray[indiceGray] = nivelGris;
	}
}

