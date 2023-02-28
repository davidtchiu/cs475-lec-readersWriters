/*
 ============================================================================
 Name        : CS475_Lec6_ReadersWriters.c
 Author      : David Chiu
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include "reader.h"
#include "writer.h"

#define NUM_READERS 10
#define NUM_WRITERS 5

void init_semaphores();

//declare semaphores in shared space
sem_t *sem_writer;
sem_t *mutex;
int reader_count = 0;

int main(int argc, char **argv) {
	int i;

	init_semaphores();

	//create consumer and producer threads
	pthread_t *reader_thread = (pthread_t*) malloc(NUM_READERS * sizeof(pthread_t));
	pthread_t *writer_thread = (pthread_t*) malloc(NUM_WRITERS * sizeof(pthread_t));
	for (i = 0; i < NUM_WRITERS; i++) {
		pthread_create(&writer_thread[i], NULL, writer, NULL);
	}
	for (i = 0; i < NUM_READERS; i++) {
		pthread_create(&reader_thread[i], NULL, reader, NULL);
	}

	//join all threads
	for (i=0; i<NUM_WRITERS; i++) {
		pthread_join(writer_thread[i], NULL);
	}
	for (i=0; i<NUM_READERS; i++) {
		pthread_join(reader_thread[i], NULL);
	}

	//close semaphores
	sem_close(sem_writer);
	sem_close(mutex);

	//delete semaphores
	sem_unlink("/writer");
	sem_unlink("/lock");

	//free up memory
	free(reader_thread);
	free(writer_thread);

	return 0;
}


/**
 * Initialize semaphores
 */
void init_semaphores() {
	//remove semaphores if they were never deleted
	sem_unlink("/writer");
	sem_unlink("/lock");

	//create them again
	sem_writer = sem_open("/writer", O_CREAT, 0777, 1);
	mutex = sem_open("/lock", O_CREAT, 0777, 1); //just a lock

	if (sem_writer == SEM_FAILED)
		printf("Failed obtaining /writer\n");
	if (mutex == SEM_FAILED)
		printf("Failed obtaining /lock\n");
}

