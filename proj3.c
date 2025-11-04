/*
    proj3.c

    This file creates producer and consumer threads
    to use a mutex and semaphores to synchronize the producer-consumer process.
    The producer will produce a random value in the buffer, and a consumer will 
    remove a value from the buffer and will specify if the value consumed is prime.

    Created by Christian Quintero on 11/03/2025
*/
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"

// prototypes
void print_command_line_args(int arg, char *argv[]);
void init_globals();

// global variabes
pthread_mutex_t mutex;          // define a mutex
sem_t empty;                    // define an empty semaphore
sem_t full;                     // define a full semaphore

int main(int argc, char *argv[]) {
    /*
        params:
            argc: int - the argument count from the CLI
            argv: char * - an array of character points (strings). Each element points to a single argument
    */

    // create the buffer
    buffer_item buffer[BUFFER_SIZE];

    // intialize the mutex and semaphores
    init_globals();







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
        This function initializes the mutex and semaphores
    */

    // initialize the empty semaphore with this process' scope and to the value 'BUFFER_SIZE'
    sem_init(&empty, 0, BUFFER_SIZE);

    // initialize the full semaphore with this process' scope and to the value 0
    sem_init(&full, 0, 0);

    // create the mutex lock with default attributes
    pthread_mutex_init(&mutex, NULL);
}
