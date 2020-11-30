#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "helper.h"
#include "defs.h"

extern int *A, *B, resultado;

void proceso_padre(int pipefd[NUM_PROC][2])
{
	register int np;
	pid_t pid;
	int estado, numproc;
	int suma_parcial;
	for (np = 0; np < NUM_PROC; np++)
	{
		pid = wait(&estado);
		numproc = estado >> 8;

		close(pipefd[numproc][1]);
		read(pipefd[numproc][0], &suma_parcial, sizeof(int));
		resultado += suma_parcial;
		printf("Termino el proceso %d con pid: %d \n", numproc, pid);
		
		close(pipefd[numproc][0]);
	}
}

void proceso_hijo(int np, int pipefd[])
{
	register int i;
	int suma = 0;
	close(pipefd[0]);
	
	for (i = np; i < N; i+=NUM_PROC) 
		suma += A[i] * B[i];
	write(pipefd[1], &suma, sizeof(int));

	close(pipefd[1]);
	exit(np);
}