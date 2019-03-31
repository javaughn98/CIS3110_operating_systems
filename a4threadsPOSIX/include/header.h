#ifndef HEADER_H
#define HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

int NITER = 1000000;
int cnt = 0;
void *Count(void *a);
sem_t sem_main;
bool isNum(char *array); 





#endif


