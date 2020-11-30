#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 16

void proceso_padre(int pipefd[NUM_PROC][2]);
void proceso_hijo(int np, int pipefd[]);

int *A, *B, *C;

void llenarArreglo(int datos[]);
void imprimirArreglo(int datos[]);
int *reservarMemoria(void);

int main()
{
	pid_t pid;
	register int np;
	int pipefd[NUM_PROC][2], edo_pipe;

	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();
	llenarArreglo(A);
	llenarArreglo(B);
	imprimirArreglo(A);
	imprimirArreglo(B);

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

	imprimirArreglo(C);

	free(A);
	free(B);
	free(C);

	return 0;
}

void proceso_padre(int pipefd[NUM_PROC][2])
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

		read(pipefd[numproc][0], C + inicio, sizeof(int) * eleBloque);
		
		printf("Termino el proceso %d con pid: %d \n", numproc, pid);
		
		close(pipefd[numproc][0]);
	}
}

void proceso_hijo(int np, int pipefd[])
{
	int eleBloque = N / NUM_PROC;
	int iniBloque = np * eleBloque;
	int finBloque = iniBloque + eleBloque;

	register int i;

	close(pipefd[0]);

	for (i = iniBloque; i < finBloque; i++)
		C[i] = A[i] * B[i];

	write(pipefd[1], C + iniBloque, sizeof(int) * eleBloque);
	close(pipefd[1]);
	exit(np);
}

void llenarArreglo(int datos[])
{
	register int n;
	for (n = 0; n < N; n++)
	{
		datos[n] = rand() % 4096;
	}
}

void imprimirArreglo(int datos[])
{
	register int n;
	for (n = 0; n < N; n++)
	{
		if (!(n % 16))
			printf("\n");
		printf("%4.d ", datos[n]);
	}
}

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
