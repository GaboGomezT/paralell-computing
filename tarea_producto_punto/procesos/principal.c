#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "procesos.h"
#include "defs.h"


int *A, *B, resultado;

int main()
{
	pid_t pid;
	register int np;
	int pipefd[NUM_PROC][2], edo_pipe;

	A = reservarMemoria();
	B = reservarMemoria();
	resultado = 0;
	llenarArreglo(A);
	llenarArreglo(B);
	imprimirArreglo(A);
	imprimirArreglo(B);

	printf("\n\nProbando procesos...\n");

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
	printf("Resultado: %d\n", resultado);
	free(A);
	free(B);

	return 0;
}
