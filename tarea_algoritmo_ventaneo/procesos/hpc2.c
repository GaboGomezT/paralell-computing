#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "procesamiento.h"
#include "archivos.h"
#include "procesos.h"
#include "defs.h"


float *ventana_hann, *producto;
int *pulse_sensor;

int main()
{
	pid_t pid;
	register int np;
	int pipefd[NUM_PROC][2], edo_pipe;

	pulse_sensor = reservarMemoria();
	ventana_hann = reservarFloatMemoria();
	producto = reservarFloatMemoria();

	genera_ventana_hann(ventana_hann);
	leer_datos(pulse_sensor, "PulseSensor.dat");

	printf("Probando procesos...\n");

	for (np = 0; np < NUM_PROC; np++)
	{
		edo_pipe = pipe(&pipefd[np][0]);
		if (edo_pipe == -1)
		{
			perror("Error al crear la tuberia...\n");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == -1)
		{
			perror("Error al crear el proceso...\n");
			exit(EXIT_FAILURE);
		}
		if (!pid)
		{
			proceso_hijo(np, &pipefd[np][0]);
		}
	}
	proceso_padre(pipefd);

	guarda_datos(producto, "producto.dat");
	guarda_datos(ventana_hann, "ventana_hann.dat");

	free(pulse_sensor);
	free(ventana_hann);
	free(producto);

	return 0;
}
