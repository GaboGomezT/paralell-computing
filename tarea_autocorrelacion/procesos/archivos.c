#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void guarda_datos(float datos[], char *archivo_nombre)
{
    FILE *ap_arch;
    register int n;

    ap_arch = fopen(archivo_nombre, "w");
    if (!ap_arch)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    for (n = 0; n < N; n++)
    {
        fprintf(ap_arch, "%f \n", datos[n]);
    }

    fclose(ap_arch);
}
void leer_datos(int datos[], char *archivo_nombre)
{
    FILE *ap_arch;
    register int n;

    ap_arch = fopen(archivo_nombre, "r");
    if (!ap_arch)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    for (n = 0; n < N; n++)
    {
        fscanf(ap_arch, "%d\n", &datos[n]);
        datos[n] -= 2200;
    }

    fclose(ap_arch);
}