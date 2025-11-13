/*
    proj3.c

    This file creates producer and consumer threads
    to use a mutex and semaphores to synchronize the producer-consumer process.
    The producer will produce a random value in the buffer, and a consumer will 
    remove a value from the buffer and will specify if the value consumed is prime.

    To run:
    gcc proj3.c buffer.c -lpthread -o proj3
    ./proj3 5 1 3 3 yes 
        This will run the program for 5 seconds with the threads sleeping a 
        max of 1 second. There will be 3 producers and 3 consumers, and output
        for each production/consumption will be shown


    Created by Christian Quintero on 11/03/2025
    Last Updated on 11/11/2025
*/
#include <stdio.h>                          // for printf()
#include <stdlib.h>                         // for atoi() and rand()
#include <unistd.h>                         // for sleep()
#include <semaphore.h>                      // to use semaphores and mutexes
#include <pthread.h>                        // to make pthreads
#include <stdbool.h>
#include "buffer.h"

// prototypes
void *run_producer(void *params);
void *run_consumer(void *params);
void print_main_stats();

// track if the simulation is still running or not
bool simulation_flag = true;

// globals
int main_sleep_time;              
int max_thread_sleep_time;  
int num_producer_threads;
int num_consumer_threads;     
bool print_buffer_snapshot;

int main(int argc, char *argv[]) {
    /*
        params:
            argc: int - the argument count from the CLI
            argv: char * - an array of character pointers (strings). Each element points to a single argument
    */

    // convert the args to ints
    main_sleep_time = atoi(argv[1]);               // argv[1] is the amount of time the main thread sleeps
    max_thread_sleep_time = atoi(argv[2]);         // argv[2] is the max amount of time a thread will sleep before producing/consuming an item
    num_producer_threads = atoi(argv[3]);          // argv[3] is the number of producer threads
    num_consumer_threads = atoi(argv[4]);          // argv[4] is the number of consumer threads

    // check the 5th argument's first character
    // if it starts with a 'y', then consider the input as "yes" (true)
    // else, consider the input false
    if( argv[5][0] == 'y') {
        print_buffer_snapshot = true;
    }
    else {
        print_buffer_snapshot = false;
    }


    // make the the thread arrays 
    pthread_t producers[num_producer_threads]; 
    pthread_t consumers[num_consumer_threads];

    // default thread attrs for all producer and consumer threads
    pthread_attr_t default_attrs;
    pthread_attr_init(&default_attrs);

    // init the mutex and semaphores
    buffer_init(print_buffer_snapshot);

    // create the producer threads
    for(int i = 0; i < num_producer_threads; i++) {
        pthread_create(&producers[i], &default_attrs, run_producer, NULL);
    }

    // crete the consumer threads
    for(int i = 0; i < num_consumer_threads; i++) {
        pthread_create(&consumers[i], &default_attrs, run_consumer, NULL);
    }

    // make the main thread sleep for the simulation duration 
    sleep(main_sleep_time);
    simulation_flag = false;

    // force the threads to post the semaphores after 
    // the simulation
    post_threads(num_producer_threads, num_consumer_threads);

    // join the producer threads
    for(int i = 0; i < num_producer_threads; i++) {
        pthread_join(producers[i], NULL);
    }
    
    // join the consumer threads
    for(int i = 0; i < num_consumer_threads; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    print_main_stats();
    print_totals();
    return 0;
}

void *run_producer(void *params) {
    /*
        run_producer
        This is the function producers run for the simulation time
    */

    // the random item
    int item = -1;

    // the number of items produced by this thread
    int produce_count = 0;

    do {
        // sleep for a random time from 1 - max_thead_sleep_time
        // before producing an item
        sleep((rand() % max_thread_sleep_time) + 1);

        // produce an int from 1 - 100
        item = (rand() % 100) + 1;

        // add the item
        buffer_insert_item(item);

    } while (simulation_flag);

    return NULL;
}

void *run_consumer(void *params) {

    do {
        // sleep for a random time from 1 - max_thead_sleep_time
        // before consuming an item
        sleep((rand() % max_thread_sleep_time) + 1);
        
        // consume an item
        buffer_remove_item();

    } while (simulation_flag);

    return NULL;
}

void print_main_stats() {
    /*
        Prints the stats that are scope specific to 
        this file
    */
    
    printf("===============================================\n");
    printf("Simulation time:\t%d\n", main_sleep_time);
    printf("Thread sleep time:\t%d\n", max_thread_sleep_time);
    printf("Producer Threads:\t%d\n", num_producer_threads);
    printf("Consumer Threads:\t%d\n", num_consumer_threads);
}
