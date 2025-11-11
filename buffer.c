/*
    buffer.c

    This file implements the buffer module.
    It provides functions to insert and remove items from a buffer.
    
    Created by Christian Quintero on 11/03/2025
    Last Updated on 11/04/2025
*/
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

bool buffer_insert_item(buffer_item item) {
    /*
        buffer_insert_item
        This function allows a producer thread to insert an item
        into the shared buffer if there is an empty space
        available.

        params:
            item: buffer_item - the item to insert into the 
        
        returns:
            bool - 1 if the item was successfully inserted into the buffer. 
                   0 otherwise.
    */



} // end buffer_insert_item

bool buffer_remove_item(buffer_item *item) {


} // end buffer_remove_item



