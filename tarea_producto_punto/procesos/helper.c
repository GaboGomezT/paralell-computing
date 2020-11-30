#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

int *reservarMemoria(void)
{
	int *mem;
	mem = (int *)malloc(sizeof(int) * N);
	if (!mem)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void llenarArreglo(int datos[])
{
	register int n;
	for (n = 0; n < N; n++)
	{
		datos[n] = rand() % 255;
	}
}

void imprimirArreglo(int datos[])
{
	register int n;
	for (n = 0; n < N; n++)
	{
		if (!(n % 16))
			printf("\n");
		printf("%4.d ", datos[n]);
	}
}