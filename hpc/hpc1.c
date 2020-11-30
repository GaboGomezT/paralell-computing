#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_HILOS 4
#define N 16
void *funHilo(void *arg);
int *A, *B, *C;
int res_suma, res_resta, res_multi, res_divi;
int num1 = 20, num2 = 4;

void llenarArreglo(int datos[]);
void imprimirArreglo(int datos[]);
int *reservarMemoria(void);

int main()
{
	int *hilo;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	register int nh;

	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();
	llenarArreglo(A);
	llenarArreglo(B);
	imprimirArreglo(A);
	imprimirArreglo(B);

	printf("Probando hilos...\n");

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		//nhs[0] = 0;
		//nhs[1] = 1;
		//nhs[2] = 2;
		//nhs[3] = 3;
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funHilo, &nhs[nh]);
	}

	for (nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **)&hilo);
		printf("El hilo %d termino \n", *hilo);
	}
	imprimirArreglo(C);
	free(A);
	free(B);
	free(C);

	return 0;
}

void *funHilo(void *arg)
{
	int nucleo = *(int *)arg;
	
	register int i;

	printf("Hilo %d en ejecucion \n", nucleo);

	for (i = nucleo; i < N; i+= NUM_HILOS)
	{
		C[i] = A[i] * B[i];
	}

	pthread_exit(arg);
}

// void *funHilo(void *arg)
// {
// 	int nucleo = *(int *)arg;
// 	int eleBloque = N / NUM_HILOS; // tamaño de bloque
// 	int iniBloque = nucleo * eleBloque;
// 	int finBloque = iniBloque + eleBloque;
// 	register int i;

// 	printf("Hilo %d en ejecucion \n", nucleo);

// 	for (i = iniBloque; i < finBloque; i++)
// 	{
// 		C[i] = A[i] * B[i];
// 	}

// 	pthread_exit(arg);
// }

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
