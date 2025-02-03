#ifndef GARBAGE_COLLECTOR
#define GARBAGE_COLLECTOR
#include <stdarg.h>
#include <stdlib.h>
#include "libft/libft.h"



void *get_adress_of_list();
void *MALLOC(int size);
void free_list()__attribute__((destructor));

#endif