/*
    buffer.c

    This file is the implementation file for buffer.h

    Created by Christian Quintero on 11/03/2025
    Last Updated on 11/11/2025
*/
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

// the shared buffer
buffer_item buffer[BUFFER_SIZE];

// the mutex
pthread_mutex_t mutex;

// the semaphores
sem_t empty;                         // tracks # of empty buffers
sem_t full;                          // tracks # of full buffers

// other globals
int count = 0;                      // tracks the number of full buffers for printing
int write_index = 0;                // the index that producers will place an item into
int read_index = 0;                 // the index that consumers will remove an item from
int total_produced = 0;             
int total_consumed = 0;
int times_buffer_full = 0;
int times_buffer_empty = 0;
bool allow_prints = false;

void buffer_init(bool print_buffer_snapshot) {
    /*
        buffer_init
        This function initializes the mutex, semaphores,
        array items, and the allow_prints flag

        params:
            print_buffer_snapshot: bool - from main, this specifies if a thread should print
                                            the status of the buffer after it has produced/consumed
    */

    allow_prints = print_buffer_snapshot;
    
    // init the mutex
    pthread_mutex_init(&mutex, NULL);

    // init the empty semaphore with default attributes to the value BUFFER_SIZE
    sem_init(&empty, 0, BUFFER_SIZE);

    // init the full semaphore to 0
    sem_init(&full, 0, 0);

    // set all the buffers to -1
    for(int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;
    }
}

void buffer_insert_item(buffer_item item){
    /*
        buffer_insert_item
        This function is ran by producers and inserts an item into the buffer 

        params:
            item: buffer_item - the item to place in the buffer
    */
    
    // wait for the empty semaphore
    sem_wait(&empty);

    // acquire the mutex lock
    pthread_mutex_lock(&mutex);

    printf("Producer %ld writes %d\n", pthread_self(), item);

    // write the item to the buffer
    buffer[write_index] = item;

    // increment the write_index
    write_index = (write_index + 1) % BUFFER_SIZE;

    // increment the count 
    count++;

    if(allow_prints) {
        print_buffer();
    }

    // if the count ever reaches 5, increase the times
    // the buffer was full
    if(count == 5) {
        times_buffer_full++;
    }

    // increment the total produced count
    total_produced++;

    // release the mutex
    pthread_mutex_unlock(&mutex);

    // signal the full semaphore
    sem_post(&full);
}

void buffer_remove_item() {
    /*
        buffer_remove_item
        This function is ran by consumers and
        removes an item from the buffer
    */

    // wait for the full semaphore
    sem_wait(&full);

    // acquire the mutex
    pthread_mutex_lock(&mutex);

    // track the item that will be consumed
    int item = buffer[read_index];

    // consume the item
    buffer[read_index] = -1;

    printf("Consumer %ld consumes %d\n", pthread_self(), item);

    // increment the read index
    read_index = (read_index + 1) % BUFFER_SIZE;

    // decrement the count
    count--;

    if(allow_prints) {
        print_buffer();
    }

    // if the count ever reaches 0, increase the times
    // the buffer was empty
    if(count == 0) {
        times_buffer_empty++;
    }

    // increment the total consumed count
    total_consumed++;

    // release the mutex
    pthread_mutex_unlock(&mutex);

    // signal the empty semaphore
    sem_post(&empty);
}

void print_buffer() {
    /*
        print_buffer
        This function prints the current buffer status
    */
    printf("---Buffers occupied:\t%d---\n", count);
    printf("%d\t%d\t%d\t%d\t%d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    printf("[0]\t[1]\t[2]\t[3]\t[4]\n\n\n");
}

void print_totals() {
    printf("Total items produced:\t%d\n", total_produced);
    printf("Total items consumed:\t%d\n", total_consumed);
    printf("Times buffer was empty:\t%d\n", times_buffer_empty);
    printf("Times buffer was full:\t%d\n", times_buffer_full);
}