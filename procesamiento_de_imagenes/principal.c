#include <stdio.h>
#include <stdlib.h>
#include "imagen.h"

int main () 
{
	bmpInfoHeader info;
	unsigned char *imagenRGB;

	imagenRGB = abrirBMP("imagenes/hoja.bmp", &info);
	displayInfo(&info);
	RGBToGray(imagenRGB, imagenGray, info.width, info.height);
	free(imagenRGB);
	return 0;
}
