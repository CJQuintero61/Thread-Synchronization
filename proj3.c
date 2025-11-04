/*

*/
#include <stdio.h>
#include <string.h>
#include "buffer.h"

// prototypes
void print_command_line_args(int arg, char *argv[]);


int main(int arg, char *argv[]) {
    /*
        params:
            arg: int - the argument count from the CLI
            argv: char * - an array of character points (strings). Each element points to a single argument
    */

    print_command_line_args(arg, argv);






}


void print_command_line_args(int arg, char *argv[]) {
    /*
        This function simply prints the command line arguments
        that were entered to the console.

        params:
            arg: int - the argument count from the CLI
            argv: char * - an array of character pointers (strings) where each element points to a single argument
    */

    int i = 0;
    while(i < arg)
    {
        printf("%s ", argv[i]);
        i++;
    } 

} // end print_command_line_args
