#include <stdio.h>
#include <pthread.h>
#include <math.h>

#include "defs.h"
#include "imagen.h"

extern unsigned char *imagenGray, *imagenSobel;
extern bmpInfoHeader info;

void *funHiloSobel(void *arg)
{
    int np = *(int *)arg;

    int eleBloque = (info.height - DIMASK) / NUM_HILOS;
	int iniBloque = np * eleBloque;
	int finBloque = iniBloque + eleBloque;

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
	for( y = iniBloque; y <= finBloque; y++ )
		for( x = 0; x <= info.width-DIMASK; x++ )
		{
			indicem = 0;
			convFila = 0;
			convCol = 0;
			for( ym = 0; ym < DIMASK; ym++ )
				for( xm = 0; xm < DIMASK; xm++ )
				{
					indicei = (y+ym)*info.width + (x+xm);
					convFila += imagenGray[indicei] * gradienteFila[indicem];
					convCol += imagenGray[indicei] * gradienteColumna[indicem++];
				}
			convFila = convFila / factor;
			convCol = convCol / factor;
			indicei = (y+1)*info.width + (x+1);
			imagenSobel[indicei] = (unsigned char)sqrt((convFila * convFila) + (convCol * convCol));
		}

    pthread_exit(arg);
}
