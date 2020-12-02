#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "archivos.h"
#include "procesamiento.h"
#include "helper.h"
#include <math.h>
#include "hilos.h"
#include "defs.h"


float *ventana_hann, *producto;
int *pulse_sensor;

int main()
{
	int *hilo;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	register int nh;

	pulse_sensor = reservarMemoria();
	ventana_hann = reservarFloatMemoria();
	producto = reservarFloatMemoria();

	genera_ventana_hann(ventana_hann);
	leer_datos(pulse_sensor, "PulseSensor.dat");

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
	guarda_datos(producto, "producto.dat");
	guarda_datos(ventana_hann, "ventana_hann.dat");

	free(pulse_sensor);
	free(ventana_hann);
	free(producto);

	return 0;
}


