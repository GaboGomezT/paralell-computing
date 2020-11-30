#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "hilos.h"
#include "defs.h"


int *A, *B, resultado;
pthread_mutex_t bloqueo;

int main()
{
	int *hilo;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	register int nh;
	
	A = reservarMemoria();
	B = reservarMemoria();
	resultado = 0;
	llenarArreglo(A);
	llenarArreglo(B);
	imprimirArreglo(A);
	imprimirArreglo(B);

	pthread_mutex_init(&bloqueo, NULL);

	printf("Probando hilos...\n");

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funHilo, &nhs[nh]);
	}

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&hilo);
		printf("El hilo %d termino \n", *hilo);
	}
	pthread_mutex_destroy(&bloqueo);

	printf("Resultado: %d\n", resultado);
	free(A);
	free(B);

	return 0;
}
