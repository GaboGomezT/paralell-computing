#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"

void GrayToRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void RGBToGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);

int main()
{
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray;

	imagenRGB = abrirBMP("imagenes/hoja.bmp", &info);

	displayInfo(&info);
	imagenGray = reservarMemoria(info.width, info.height);
	RGBToGray(imagenRGB, imagenGray, info.width, info.height);

	// brilloImagen(imagenGray, info.width, info.height);

	GrayToRGB(imagenRGB, imagenGray, info.width, info.height);

	guardarBMP("hoja-gris.bmp", &info, imagenRGB);
	free(imagenRGB);
	free(imagenGray);
	return 0;
}

void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int indiceGray;
	int pixel;

	for (indiceGray = 0; indiceGray < (height * width); indiceGray++)
	{
		pixel = imagenGray[indiceGray] + 70;
		imagenGray[indiceGray] = (pixel > 255) ? 255 : (unsigned char)pixel;
	}
}

void GrayToRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	int indiceRGB, indiceGray;
	for (indiceRGB = 0, indiceGray = 0; indiceGray < (height * width); indiceRGB += 3, indiceGray++)
	{
		imagenRGB[indiceRGB] = imagenGray[indiceGray];
		imagenRGB[indiceRGB + 1] = imagenGray[indiceGray];
		imagenRGB[indiceRGB + 2] = imagenGray[indiceGray];
	}
}

void RGBToGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	unsigned char nivelGris;
	int indiceRGB, indiceGray;
	for (indiceRGB = 0, indiceGray = 0; indiceGray < (height * width); indiceRGB += 3, indiceGray++)
	{
		nivelGris = (30 * imagenRGB[indiceRGB] + 59 * imagenRGB[indiceRGB + 1] + 11 * imagenRGB[indiceRGB + 2]) / 100;
		imagenGray[indiceGray] = nivelGris;
	}
}

unsigned char *reservarMemoria(uint32_t width, uint32_t height)
{
	unsigned char *imagen;

	imagen = (unsigned char *)malloc(width * height * sizeof(unsigned char));

	if (imagen == NULL)
	{
		perror("Error al asignar memoria a la imagen.");
		exit(EXIT_FAILURE);
	}
	return imagen;
}