#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _list list;
typedef struct _problemlist problemlist;
typedef struct _sizelist sizelist;
typedef struct _problemsol problemsol;

struct _list{
	char *word;
	list *next;
};

struct _problemlist{
	int order;
	char *pal1;
  char *pal2;
	int maxmut;
	problemlist *next;
};

struct _sizelist{
	int size;
	sizelist *next;
};

struct _problemsol{
	int weight;
	list *path;
	problemsol *next;
};

list *newlist(char *word, list *next);

problemlist *newproblemlist(int order, char *pal1, char *pal2, int maxmut, problemlist *next);

sizelist *newsizelist(int size, sizelist *next);

problemsol *newproblemsol(int weight, list *path);

problemsol *newspecialsol(int weight, list *path, problemsol *next);

#endif
