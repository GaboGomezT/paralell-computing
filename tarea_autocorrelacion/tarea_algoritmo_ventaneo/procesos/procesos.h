#ifndef PROCESOS_H
#define PROCESOS_H

#include "defs.h"

void proceso_padre(int pipefd[NUM_PROC][2], int q_correlacion);
void proceso_hijo(int np, int pipefd[], int q_correlacion);

#endif