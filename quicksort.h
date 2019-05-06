#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap_str_ptrs(char **arg1, char **arg2);

void quicksort_strs(char *args[], unsigned int len);

#endif
