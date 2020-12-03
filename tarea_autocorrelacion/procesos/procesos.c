#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "procesamiento.h"
#include "archivos.h"
#include "defs.h"

extern float *ventana_hann, *producto, *correlacion;
extern int *pulse_sensor;

void proceso_padre(int pipefd[NUM_PROC][2], int q_correlacion)
{
	register int np;
	pid_t pid;
	int estado, numproc;
	int eleBloque = N / NUM_PROC, inicio;

	for (np = 0; np < NUM_PROC; np++)
	{
		pid = wait(&estado);
		numproc = estado >> 8;

		inicio = numproc * eleBloque;

		close(pipefd[numproc][1]);

		if (q_correlacion)
		{
			read(pipefd[numproc][0], correlacion + inicio, sizeof(float) * eleBloque);
		}
		else
		{
			read(pipefd[numproc][0], producto + inicio, sizeof(float) * eleBloque);
		}

		printf("Termino el proceso %d con pid: %d \n", numproc, pid);

		close(pipefd[numproc][0]);
	}
}

void proceso_hijo(int np, int pipefd[], int q_correlacion)
{
	int eleBloque = N / NUM_PROC;
	int iniBloque = np * eleBloque;
	int finBloque = iniBloque + eleBloque;

	register int i;

	close(pipefd[0]);
	if (q_correlacion)
	{
		register int n;
		int sum;
		for (i = iniBloque; i < finBloque; i++)
		{
			sum = 0;
			for (n = 1; n < N; n++)
			{
				sum += producto[n] * producto[n - i];
			}
			correlacion[i] = sum;
		}

		write(pipefd[1], correlacion + iniBloque, sizeof(float) * eleBloque);
	}
	else
	{
		for (i = iniBloque; i < finBloque; i++)
			producto[i] = ventana_hann[i] * pulse_sensor[i];

		write(pipefd[1], producto + iniBloque, sizeof(float) * eleBloque);
	}

	close(pipefd[1]);
	exit(np);
}
