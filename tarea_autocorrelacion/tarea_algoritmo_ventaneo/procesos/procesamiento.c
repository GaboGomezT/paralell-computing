#include <stdio.h>
#include <math.h>
#include "defs.h"
extern int *ventana_hann;

void genera_ventana_hann(float ventana_hann[])
{
    register int n;
    for (n = 0; n < N; n++)
    {
        ventana_hann[n] = 0.5 - 0.5 * cosf((2 * n * M_PI) / (N - 1));
    }
}