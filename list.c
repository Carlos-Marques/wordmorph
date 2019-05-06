#include "list.h"

list *newlist(char *word, list *next){
  list *newnode;

  newnode = (list *) malloc(sizeof(list));

  newnode->word = (char *) malloc(sizeof(char)*strlen(word));

  strcpy(newnode->word, word);

  newnode->next = next;

  return newnode;

}

problemlist *newproblemlist(int order, char *pal1, char *pal2, int maxmut, problemlist *next){
  problemlist *newsizenode;

  newsizenode = (problemlist *) malloc(sizeof(problemlist));

  newsizenode->order = order;

  newsizenode->pal1 = (char *) malloc(sizeof(char)*strlen(pal1));
  strcpy(newsizenode->pal1, pal1);

  newsizenode->pal2 = (char *) malloc(sizeof(char)*strlen(pal2));
  strcpy(newsizenode->pal2, pal2);

  newsizenode->maxmut = maxmut;

  newsizenode->next = next;

  return newsizenode;

}

sizelist *newsizelist(int size, sizelist *next){
  sizelist *newsizenode;

  newsizenode = (sizelist *) malloc(sizeof(sizelist));

  newsizenode->size = size;

  newsizenode->next = next;

  return newsizenode;

}

problemsol *newproblemsol(int weight, list *path){
  problemsol *newproblemsolnode;

  newproblemsolnode = (problemsol *) malloc(sizeof(problemsol));

  newproblemsolnode->weight = weight;

  newproblemsolnode->path = path;

  return newproblemsolnode;

}

problemsol *newspecialsol(int weight, list *path, problemsol *next){
  problemsol *newproblemsolnode;

  newproblemsolnode = (problemsol *) malloc(sizeof(problemsol));

  newproblemsolnode->weight = weight;

  newproblemsolnode->path = path;

  newproblemsolnode->next = next;

  return newproblemsolnode;

}
