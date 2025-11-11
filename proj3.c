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
#include <stdlib.h>                         // for atoi() and rand()
#include <unistd.h>                         // for sleep()
#include <semaphore.h>                      // to use semaphores and mutexes
#include <pthread.h>                        // to make pthreads
#include <stdatomic.h>                      // for _Atomic 
#include "buffer.h"

// prototypes
void print_command_line_args(int arg, char *argv[]);
void init_globals();
void *produce_item(void *params);
void *consume_item(void *params);
void *do_nothing(void *params);

// global variabes
pthread_mutex_t mutex;              // define a mutex
sem_t empty;                        // counts the amount of empty slots
sem_t full;                         // counts the amount of full slots
buffer_item buffer[BUFFER_SIZE];    // define the shared buffer 
bool simulation_flag = true;        // to signal if the simulation is still running or not
_Atomic int count = 0;              // to index the circular array

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
        pthread_create(&producers[i], &default_attrs, produce_item, NULL);
    }

    // crete the consumer threads
    for(int i = 0; i < num_consumer_threads; i++) {
        pthread_create(&consumers[i], &default_attrs, do_nothing, NULL);
    }

    // make the main thread sleep for the simulation duration 
    sleep(main_sleep_time);
    simulation_flag = false;

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

void *produce_item(void *params) {
    /*
        produce_item:
        This is the function producer threads will run.
        It will:
            1. produce a random integer 
            2. wait for the empty semaphore
            3. wait for the mutex lock
            4. add the item to the buffer
            5. release the mutex lock
            6. signal the full mutex

        params:
            params: *void - the thread parameters. None
    */
    int max = 100;                      // maximum value in the random number range
    int min = 1;                        // minimum value in the random number range
    bool success = false;               // the exit status of inserting the item
    pthread_t id = pthread_self();      // get the thread id
    
    // infite loop for the duration of the 
    // simulation
    do {
        // produce a random int from [1-100]
        buffer_item item = (rand() % max) + min;

        // wait for the empty semaphore
        sem_wait(&empty);

        // acquire the mutex
        pthread_mutex_lock(&mutex);

        // place the item in the buffer
        success = buffer_insert_item(item);

        // increment the count
        count++;

        if(success) {
            printf("Producer %ld produces %d\n", id, item);
        }
        else {
            printf("Producer %ld failed to insert %d\n", id, item);
        }

        // release the mutex lock
        pthread_mutex_unlock(&mutex);

        // update the full semaphore
        sem_post(&full);

    } while(simulation_flag);

}


bool buffer_insert_item(buffer_item item) {
    buffer[ (count %  BUFFER_SIZE) ] = item;
}

void *do_nothing(void *params) {
    ;
}