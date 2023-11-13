#ifndef SHELL_H
#define SHELL_H
/*
 *The above commands prevent the header file from being included more than once.
 */
#include <stdio.h>
#include <stdlib.h>//provide function for memory management and other general utilities.
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/*
 *the  include commands function provide the necessary declarations for functions and data types used in the header file. 
 */
void display_prompt(void);//provides function for input and output
void joelprint(const char *enter_valhalla);//
void read_command(char *command, size_t size);
void execute_command(const char *command);




#endif /* SHELL_H */
