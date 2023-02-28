/*
 * writer.c
 *
 *  Created on: Mar 27, 2016
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "writer.h"

extern sem_t *sem_writer;

/**
 * Consumer threads run this. Consumes 10 items.
 */
void* writer(void *args) {
	sleep(1);
	printf("Writer created id=%lu\n", pthread_self());
	sem_wait(sem_writer);
	printf("Writer is writing id=%lu\n", pthread_self());
	sleep(1);
	printf("Write done writing id=%lu\n", pthread_self());
	sem_post(sem_writer);

	return NULL;
}
