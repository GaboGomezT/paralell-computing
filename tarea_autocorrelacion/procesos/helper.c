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
float *reservarFloatMemoria(void)
{
	float *mem;
	mem = (float *)malloc(sizeof(float) * N);
	if (!mem)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}
	return mem;
}