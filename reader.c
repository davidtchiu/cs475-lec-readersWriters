/*
 * reader.c
 *
 *  Created on: Mar 27, 2016
 *      Author: dchiu
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include "reader.h"

extern sem_t *sem_writer;
extern sem_t *mutex;
extern int reader_count;


/**
 * Producer threads run this. Produces 10 items.
 */
void* reader(void *args) {
	sleep(1);
	printf("Reader created id=%lu\n", pthread_self());

	sem_wait(mutex);	//lock
	reader_count++;

	if (reader_count == 1) {
		//first reader in, so I might have to wait on a writer
		sem_wait(sem_writer);
	}
	sem_post(mutex);	//unlock

	printf("Reader is reading id=%lu\n", pthread_self());
	sleep(1);
	printf("Reader is done reading id=%lu\n", pthread_self());

	sem_wait(mutex);	//lock
	reader_count--;

	if (reader_count == 0) {
		//I'm the last reader! Let in a writer
		sem_post(sem_writer);
	}
	sem_post(mutex);	//unlock

	return NULL;
}

