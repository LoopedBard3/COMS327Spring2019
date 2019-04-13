#ifndef IO_H
# define IO_H

#include "object.h"

class dungeon;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon *d);
void io_handle_input(dungeon *d);
void io_queue_message(const char *format, ...);

//Assignment 1.09 methods
void show_inventory(dungeon *d);
void display_desc(object* list[], int pos);
#endif
