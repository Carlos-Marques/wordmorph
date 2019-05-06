#ifndef READFILE_H
#define READFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "quicksort.h"
#include "dif.h"
#include "dijkstra.h"

void findpos(int *pos, char **wcache, int min, int nword, char *pal); /* uses recursive binary search to find word position in given array */
char **listtoarray(int nword, list *header);  /* allocs array with size of number of words, tranfers the words from the list to the array and quicksorts */
void readfile(char *dic, char *pal);

#endif
