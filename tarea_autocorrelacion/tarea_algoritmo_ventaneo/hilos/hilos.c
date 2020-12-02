#include <stdio.h>
#include <pthread.h>

#include "defs.h"

extern int *pulse_sensor;
extern float *producto, *ventana_hann, *correlacion;
extern int q_correlacion;
void *funHilo(void *arg)
{
	int nucleo = *(int *)arg;

	register int i;

	printf("Hilo %d en ejecucion \n", nucleo);

	if (q_correlacion)
	{
		register int n;
		float sum;
		for (i = nucleo; i < N; i += NUM_HILOS)
		{
			sum = 0;
			for (n = 1; n < N; n++)
			{
				sum += producto[n] * producto[n - i];
			}
			correlacion[i] = sum;
		}
	}
	else
	{
		for (i = nucleo; i < N; i += NUM_HILOS)
		{
			producto[i] = ventana_hann[i] * pulse_sensor[i];
		}
	}

	pthread_exit(arg);
}