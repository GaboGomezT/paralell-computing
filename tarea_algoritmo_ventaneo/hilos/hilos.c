#include <stdio.h>
#include <pthread.h>

#include "defs.h"

extern int* pulse_sensor;
extern float *producto, *ventana_hann;

void *funHilo(void *arg)
{
	int nucleo = *(int *)arg;

	register int i;

	printf("Hilo %d en ejecucion \n", nucleo);

	for (i = nucleo; i < N; i += NUM_HILOS)
	{
		producto[i] = ventana_hann[i] * pulse_sensor[i];
	}

	pthread_exit(arg);
}