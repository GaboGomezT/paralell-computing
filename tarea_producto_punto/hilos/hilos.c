#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "defs.h"

extern int *A, *B, resultado;
extern pthread_mutex_t bloqueo;

void *funHilo(void *arg)
{
	int nucleo = *(int *)arg;
	int eleBloque = N / NUM_HILOS; // tamaño de bloque
	int iniBloque = nucleo * eleBloque;
	int finBloque = iniBloque + eleBloque;
	register int i;
	int suma_parcial = 0;
	printf("Hilo %d en ejecucion \n", nucleo);

	for (i = iniBloque; i < finBloque; i++)
	{
		suma_parcial += A[i] * B[i];
	}
	pthread_mutex_lock(&bloqueo);
	resultado += suma_parcial;
	pthread_mutex_unlock(&bloqueo);
	pthread_exit(arg);
}