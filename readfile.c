#include "readfile.h"

void findpos(int *pos, char **wcache, int min, int nword, char *pal){ /* uses recursive binary search to find word position in given array */
  int len = nword-min;  /* len of array is nword minus min selecting part of array to work on */

  if(strcmp(pal, wcache[min+(len/2)]) == 0){  /* if checked word is equal to searched word then we found pos */
    *pos = min+(len/2); /* stores pos */
  }
  else if(strcmp(pal, wcache[min+(len/2)]) < 0){  /* if checked word is before in order than searched word then check this portion of array from beginning to the pos of this word */
    findpos(pos, wcache, min, min+(len/2), pal);
  }
  else if(strcmp(pal, wcache[min+(len/2)]) > 0){  /* if checked word is after in order than searched word then check this portion of array from the pos to the end */
    findpos(pos, wcache, min+(len/2), nword, pal);
  }
}

char **listtoarray(int nword, list *header){  /* allocs array with size of number of words, tranfers the words from the list to the array and quicksorts */
  char **warray = NULL; /* array of all the words with same size as given words */
  int wcounter = 0;     /* aux counter to run through list of words */
  list *aux = NULL;     /* aux node of list to help creat new items to add to list */

  warray = (char **) malloc(sizeof(char*)*nword); /* alloc array with the number of words given */

  while (header != NULL) {            /* transfer list to the new array */
    warray[wcounter] = header->word;  /* store word in array */
    wcounter++;                       /* count another word */
    aux = header->next;               /* store next pointer */
    free(header);                     /* free current head of stack */
    header = aux;                     /* point stack head to stored value */
  }

  quicksort_strs(warray, nword);  /* quicksort the array */

  return warray;  /* return created array */
}

void readfile(char *dic, char *pal){                      /* solver */
  FILE *fdic = NULL, *fpal = NULL, *fpath = NULL;         /* file variable declaration for .dic, .pal and .path files */
  char *path = NULL;                                      /* name of result file */
  int type = 0;                                           /* type of process */
  char pal1[100], pal2[100], palaux[100];                 /* words to process and aux word to run through .dic file */
  unsigned int sword = 0;                                 /* size of word to be processed */
  int auxcounter = 0, auxcounterarray = 0;                /* counters to help initialize cache with NULL value and free cache */
  int maxweight = 0;                                      /* maxweight per word mutation */
  problemlist *pcache[100], *pcacheaux;                   /* problem cache for each size of word and aux pointer */
  int ordercounter = 0;                                   /* counter for order of problems */
  int maxmutweight[100];                                  /* max mutation weight per word size */
  sizelist *psizes = NULL, *paux = NULL;                  /* list of problem sizes and aux pointer */
  list *header = NULL, *listaux = NULL, *auxlist = NULL;  /* list of words extracted from .dic file, list of words for problem solution and aux pointer */
  int nword = 0;                                          /* number of words in .dic file */
  char **wcache = NULL;                                   /* array of pointers to cache words of same size for solving problems of that size */
  int pos1 = 0, pos2 = 0;                                 /* pos of first and second word given */
  struct Graph* graph;                                    /* pointer to graph */
  int wdif = 0;                                           /* weight difference between two words */
  problemsol **sol, *spsol = NULL;                        /* array of solutions, list of special solutions */

  for(auxcounter = 0; auxcounter < 100; auxcounter++){    /* initialize problem cache as NULL and maxmutweight with the value 0 */
    pcache[auxcounter] = NULL;
    maxmutweight[auxcounter] = 0;
  }

  fpal = fopen(pal, "r"); /* opening .pal file to read */

  while(fscanf(fpal, "%s %s %d", pal1, pal2, &type) != EOF) {
    sword = strlen(pal1); /* measures size of pal1 */

    maxweight = type*type; /* based on given max number of words calculates max weight of mutation for given problem */

    wdif = dif(pal1, pal2, sword);  /* calculates difference between the two words to catch cases where the solution is imediate */

    if(wdif > 1){ /* if maxweight > 1 then solution is not imediate */
      if(pcache[sword] == NULL){              /* checks if the problem cache for that word size has not been alloced */
        psizes = newsizelist(sword, psizes);  /* allocs size stack with the size of given word */
      }

      pcache[sword] = newproblemlist(ordercounter, pal1, pal2, maxweight, pcache[sword]); /* pushes stack of problems with the order, pal1, pal2 and maxweight of given problem */

      if(maxmutweight[sword] < maxweight){  /* checks if given maxweight is smaller than the max stored value for that word size */
        maxmutweight[sword] = maxweight;    /* changes stored max to new max */
      }
    }
    else{ /*  if the solution is imediate */
      auxlist = newlist(pal2, auxlist); /* pushes stack of words with pal2 */
      auxlist = newlist(pal1, auxlist); /* pushes stack of words with pal1 */

      spsol = newspecialsol(ordercounter, auxlist, spsol);  /* pushes specialsol stack with new solution storing order of solution and a solution path */

      auxlist = NULL; /* resets the auxlist to NULL so it can be used without problems */
    }

    ordercounter++; /* increases counter to account for another problem */
  }

  fclose(fpal);   /* close .pal file */

  sol = (problemsol **) malloc(sizeof(problemsol *)*ordercounter);  /* creats a sol array with the size of all problems */

  while(spsol != NULL){         /* while specialsol isn't empty transfer special solution to the sol array */
    sol[spsol->weight] = spsol; /* stores specialsol in the right order */

    if(strcmp(sol[spsol->weight]->path->word, sol[spsol->weight]->path->next->word) == 0){  /* checks if solution is 0 */
      sol[spsol->weight]->weight = 0; /* changes sol weight to correct solution 0 */
    }
    else{ /* if sol is not 0 then the only other special case is solution 1 */
      sol[spsol->weight]->weight = 1; /* changes sol weight to correct solution 1 */
    }

    spsol = spsol->next;  /* advances in specialsol stack */
  }

  while(psizes != NULL){  /* while problem size stack is not NULL */
    sword = psizes->size; /* gets size of words */

    fdic = fopen(dic, "r"); /* opening .dic file to read */

    nword = 0;                                  /* set nword as 0 to initialize counting */
    while (fscanf(fdic, "%s", palaux) != EOF) { /* reads .dic file searching for words the same size as given word */
      if(strlen(palaux) == sword){              /* if the word from .dic is the same size as given word then it adds it to word list */
        nword++;                                /* count one more word of the same size */

        header = newlist(palaux, header);       /* creates new node with .dic word and adds it to the list */
      }
    }

    fclose(fdic); /* close .dic file */

    wcache = listtoarray(nword, header);  /* allocs array with size of number of words, tranfers the words from the list to the array and quicksorts */
    header = NULL;  /* resets header pointer for later uses */

    graph = createGraph(nword); /* creates a graph for this specif size with nword nodes */

    maxweight = maxmutweight[sword]+1;  /* gets maxweight of mutation from the array and adds 1 to simplify operations comparison */

    for(auxcounter = 0; auxcounter < nword; auxcounter++){  /* calculates every difference between words of given size and adds edges to the graph that are below the maxweight for that size */
      for(auxcounterarray = auxcounter; auxcounterarray < nword; auxcounterarray++){
        wdif = dif(wcache[auxcounter], wcache[auxcounterarray], sword);
        if(wdif < maxweight){
          addEdge(graph, auxcounter, auxcounterarray, wdif);
        }
      }
    }

    while(pcache[sword] != NULL){ /* while stack of given size is not NULL */
      findpos(&pos1, wcache, 0, nword, pcache[sword]->pal1);  /* calculates pos of first word in warray */
      findpos(&pos2, wcache, 0, nword, pcache[sword]->pal2);  /* calculates pos of second word in warray */

      sol[pcache[sword]->order] = (problemsol *) malloc(sizeof(problemsol));  /* allocs a sol with the order of this problem in sol array */

      dijkstra(graph, pos1, pos2, sol, wcache, pcache[sword]->order, pcache[sword]->maxmut);  /* performs the dijkstra algorithm in graph with src being the pos of first word and dest being the pos of second word also takes sol to store calculated path and weight, wcache to calculate paths based on indexes, problem order store solution in right order and problem maxweight to only use edges that don't violate that value */

      free(pcache[sword]->pal1);  /* free problem pal1 */
      free(pcache[sword]->pal2);  /* free problem pal2 */

      pcacheaux = pcache[sword]->next;  /* store next pointer */
      free(pcache[sword]);              /* free current head of stack */
      pcache[sword] = pcacheaux;        /* point stack head to stored value */
    }

    for(auxcounter = 0; auxcounter < nword; auxcounter++){  /* free all stored words from .dic */
      free(wcache[auxcounter]);
    }

    free(wcache);             /* free array of words */
    freegraph(graph, nword);  /* free graph */

    paux = psizes->next;      /* store next pointer */
    free(psizes);             /* free current head of stack */
    psizes = paux;            /* point stack head to stored value */
  }

  path = strtok(pal, ".");  /* getting name of .pal file to make .path file */
  strcat(path, ".path");    /* concatenating name of path file with extention .path */

  fpath = fopen(path, "w"); /* opening .path file to write */

  for(auxcounter = 0; auxcounter < ordercounter; auxcounter++){ /* print all solution to .path file */

    fprintf(fpath, "%s %d\n", sol[auxcounter]->path->word, sol[auxcounter]->weight);  /* first line of solution as first word of path and solution weight */

    free(sol[auxcounter]->path->word);      /* free first word */
    listaux = sol[auxcounter]->path->next;  /* store next pointer */
    free(sol[auxcounter]->path);            /* free current head of stack */
    sol[auxcounter]->path = listaux;        /* point stack head to stored value */

    while(sol[auxcounter]->path != NULL){ /* while path stack isn't empty print rest of calculated path */
      fprintf(fpath, "%s\n", sol[auxcounter]->path->word);  /* print word */

      free(sol[auxcounter]->path->word);  /* free word */

      listaux = sol[auxcounter]->path->next;  /* store next pointer */
      free(sol[auxcounter]->path);            /* free current head of stack */
      sol[auxcounter]->path = listaux;        /* point stack head to stored value */
    }

    fprintf(fpath, "\n"); /* print /n to organize .path file */

    free(sol[auxcounter]);  /* free solution */
  }

  free(sol);  /* free array of all solutions */

  fclose(fpath);  /* close .path file */
}
