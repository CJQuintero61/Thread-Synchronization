/*
    proj3.c

    This file creates producer and consumer threads
    to use a mutex and semaphores to synchronize the producer-consumer process.
    The producer will produce a random value in the buffer, and a consumer will 
    remove a value from the buffer and will specify if the value consumed is prime.

    Created by Christian Quintero on 11/03/2025
    Last Updated on 11/04/2025
*/
#include <stdio.h>                          // for printf()
#include <stdlib.h>                         // for atoi()
#include <unistd.h>                         // for sleep()
#include <semaphore.h>                      // to use semaphores and mutexes
#include <pthread.h>                        // to make pthreads
#include "buffer.h"

// prototypes
void print_command_line_args(int arg, char *argv[]);
void init_globals();
void *hello_producer(void *params);
void *hello_consumer(void *params);

// global variabes
pthread_mutex_t mutex;              // define a mutex
sem_t empty;                        // define an empty semaphore
sem_t full;                         // define a full semaphore
sem_t count;                        // define a counting semaphore
buffer_item buffer[BUFFER_SIZE];    // define the shared buffer 

int main(int argc, char *argv[]) {
    /*
        params:
            argc: int - the argument count from the CLI
            argv: char * - an array of character pointers (strings). Each element points to a single argument
    */

    // convert the args to ints
    int main_sleep_time = atoi(argv[1]);               // argv[1] is the amount of time the main thread sleeps
    int max_thread_sleep_time = atoi(argv[2]);         // argv[2] is the max amount of time a thread will sleep before producing/consuming an item
    int num_producer_threads = atoi(argv[3]);          // argv[3] is the number of producer threads
    int num_consumer_threads = atoi(argv[4]);          // argv[4] is the number of consumer threads

    // intialize the mutex, semaphore, and the shared buffer
    init_globals();
    
    // make the the thread arrays 
    pthread_t producers[num_producer_threads]; 
    pthread_t consumers[num_consumer_threads];

    // default thread attrs for all producer and consumer threads
    pthread_attr_t default_attrs;
    pthread_attr_init(&default_attrs);

    // create the producer threads
    for(int i = 0; i < num_producer_threads; i++) {
        pthread_create(&producers[i], &default_attrs, hello_producer, NULL);
    }

    // crete the consumer threads
    for(int i = 0; i < num_consumer_threads; i++) {
        pthread_create(&consumers[i], &default_attrs, hello_consumer, NULL);
    }

    // make the main thread sleep for the simulation duration 
    sleep(main_sleep_time);

    // join the producer threads
    for(int i = 0; i < num_producer_threads; i++) {
        pthread_join(producers[i], NULL);
    }
    
    // join the consumer threads
    for(int i = 0; i < num_consumer_threads; i++) {
        pthread_join(consumers[i], NULL);
    }

    return 0;
}


void print_command_line_args(int argc, char *argv[]) {
    /*
        This function simply prints the command line arguments
        that were entered to the console.

        params:
            argc: int - the argument count from the CLI
            argv: char * - an array of character pointers (strings) where each element points to a single argument
    */

    // arg0 = file name
    // arg1 = sleep time for the main thread (simulation time)
    // arg2 = max time a producer/consumer will sleep before producing or consuming an item
    // arg3 = number of producer threads
    // arg4 = number of consumer threads
    // arg5 = yes/no to display the output

    int i = 0;
    while(i < argc)
    {
        printf("Arg %d: %s\n", i, argv[i]);
        i++;
    } 

} // end print_command_line_args

void init_globals()
{
    /*
        This function initializes the mutex, semaphores, and the buffer array
    */

    // initialize the empty semaphore with this process' scope and to the value 'BUFFER_SIZE'
    sem_init(&empty, 0, BUFFER_SIZE);

    // initialize the full semaphore with this process' scope and to the value 0
    sem_init(&full, 0, 0);

    // create the mutex lock with default attributes
    pthread_mutex_init(&mutex, NULL);

    // initialize each buffer item to empty 
    for(int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;
    }

} // end init_globals

void *hello_producer(void *params) {
    /*
        A simple debugging function
    */
    printf("Hello from a producer thread!\n");
    pthread_exit(0);

} // end hello_producer

void *hello_consumer(void *params) {
    /*
        A simple debugging function
    */
    printf("Hello from a consumer thread!\n");
    pthread_exit(0);

} // end hello_consumer
