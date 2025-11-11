/*
    buffer.h

    This file is the header file for the buffer module.
    It defines the buffer size, the buffer item type, and the prototypes
    for the buffer functions.

    Created by Christian Quintero on 11/03/2025
    Last Updated on 11/04/2025
*/

#ifndef BUFFER_H
#define BUFFER_H

// imports
#include <stdbool.h>                    // to use the bool data type

// the size of the buffer
// the buffer can then hold items from 0 to BUFFER_SIZE - 1
#define BUFFER_SIZE 5

// typedef for the buffer item
// this allows buffer_item to be used in place of an int like a synonym
typedef int buffer_item;

// prototypes for buffer functions
bool buffer_insert_item(buffer_item item);
bool buffer_remove_item(buffer_item *item);

#endif // BUFFER_H