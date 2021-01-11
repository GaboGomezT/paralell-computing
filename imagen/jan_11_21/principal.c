#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imagen.h"
#define DIMASK 5

void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height );
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height );
void brilloImagen( unsigned char *imagenGray, uint32_t width, uint32_t height );
void filtroPB ( unsigned char *imagenG, unsigned char *imagenF, uint32_t width, uint32_t height );

unsigned char * reservarMemoria( uint32_t width, uint32_t height );

int main( )
{
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray, *imagenFiltrada;

	imagenRGB = abrirBMP("huella1.bmp", &info );

	displayInfo( &info );
	imagenGray = reservarMemoria( info.width, info.height );
	imagenFiltrada = reservarMemoria( info.width, info.height );
	memset( imagenFiltrada, 255, info.width*info.height );

	RGBToGray( imagenRGB, imagenGray, info.width, info.height );

	//brilloImagen( imagenGray, info.width, info.height );
	filtroPB( imagenGray, imagenFiltrada, info.width, info.height );

	GrayToRGB( imagenRGB, imagenFiltrada, info.width, info.height );

	guardarBMP("huella1Gauss.bmp", &info, imagenRGB );

	free( imagenRGB );
	free( imagenGray );
	free( imagenFiltrada );

	return 0;
}

void filtroPB( unsigned char *imagenG, unsigned char *imagenF, uint32_t width, uint32_t height )
{
	register int x, y, xm, ym;
	int indicem, indicei, conv, factor = 330;
	int kernelGaussiano[DIMASK*DIMASK] = {
		1, 4,   7,  4, 1,
		4, 20, 33, 20, 4,
		7, 33, 54, 33, 7,
		4, 20, 33, 20, 4,
		1, 4,   7,  4, 1,
	};
	for( y = 0; y <= height-DIMASK; y++ )
		for( x = 0; x <= width-DIMASK; x++ )
		{
			indicem = 0;
			conv = 0;
			for( ym = 0; ym < DIMASK; ym++ )
				for( xm = 0; xm < DIMASK; xm++ )
				{
					indicei = (y+ym)*width + (x+xm);
					conv += imagenG[indicei] * kernelGaussiano[indicem++];
				}
			conv = conv / factor;
			indicei = (y+2)*width + (x+2);
			imagenF[indicei] = conv;
		}
}

void brilloImagen( unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	register int indiceGray;
	int pixel;

	for( indiceGray = 0; indiceGray < (height*width); indiceGray++ )
	{
		pixel = imagenGray[indiceGray] + 70;
		imagenGray[indiceGray] = (pixel > 255)? 255 : (unsigned char)pixel;
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
	//	nivelGris = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2]) / 3;
		nivelGris = (30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2]) / 100;
		// 0.30 = 30 / 100
		// 0.59 = 59 / 100
		// 0.11 = 11 / 100
		imagenGray[indiceGray] = nivelGris;
	}
}

/*
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	register int x, y;
	int indiceRGB, indiceGray;

	for( y = 0; y < height; y++ )
		for( x = 0; x < width; x++ )
		{
			indiceGray = (y*width + x);
			indiceRGB =  indiceGray * 3;
			imagenRGB[indiceRGB]   = imagenGray[indiceGray];
			imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
			imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
		}
}

void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	register int x, y;
	unsigned char nivelGris;
	int indiceRGB, indiceGray;

	for( y = 0; y < height; y++ )
		for( x = 0; x < width; x++ )
		{
			indiceGray = (y*width + x);
			indiceRGB =  indiceGray * 3;
		//	nivelGris = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2]) / 3;
			nivelGris = (30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2]) / 100;
		// 0.30 = 30 / 100
		// 0.59 = 59 / 100
		// 0.11 = 11 / 100
			imagenGray[indiceGray] = nivelGris;
		}
}*/

unsigned char * reservarMemoria( uint32_t width, uint32_t height )
{
	unsigned char *imagen;

	imagen = (unsigned char *)malloc( width*height*sizeof(unsigned char) );
	if( imagen == NULL )
	{
		perror("Error al asignar memoria a la imagen");
		exit(EXIT_FAILURE);
	}

	return imagen;
}
