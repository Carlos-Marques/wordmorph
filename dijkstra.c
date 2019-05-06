#include "dijkstra.h"

struct AdjListNode* newAdjListNode(int dest, int weight){ /* creates new node for adjlist with given dest and weight */
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode)); /* allocs new node */
    newNode->dest = dest;     /* store destination */
    newNode->weight = weight; /* store weight */
    newNode->next = NULL;     /* init next as NULL */
    return newNode;           /* return created node */
}

struct Graph* createGraph(int V){ /* creates graph based on number of vertices given */
    int i = 0;  /* i to help init the Adjlist as NULL */
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph)); /* allocs graph */
    graph->V = V; /* stores number of vertices given */

    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));  /* allocs Adjlist with size of all vertices */

    for (i = 0; i < V; ++i) /* inits all Adjlists as NULL */
        graph->array[i].head = NULL;

    return graph; /* return created graph */
}

struct MinHeapNode* newMinHeapNode(int v, int dist){  /* adds new node to heap with destination v and weight dist */
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode)); /* allocs new heap node */
    minHeapNode->v = v;       /* stores destination */
    minHeapNode->dist = dist; /* stores weight */
    return minHeapNode;       /* returns created heap node */
}

struct MinHeap* createMinHeap(int capacity){  /* creates heap of given capacity */
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));  /* allocs heap */
    minHeap->pos = (int *)malloc(capacity * sizeof(int));   /* allocs array of positions with size of capacity */
    minHeap->size = 0;                                      /* inits size of heap as 0 */
    minHeap->capacity = capacity;                           /* stores capacity value */
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*)); /* allocs heap nodes based on capacity */
    return minHeap; /* returns created heap */
}

void addEdge(struct Graph* graph, int src, int dest, int weight){ /* adds edge to given graph with src, dest and weight */
    struct AdjListNode* newNode = newAdjListNode(dest, weight);   /* allocs new Adjlist node with given dest and weight */
    newNode->next = graph->array[src].head;                       /* points new Adjlist node to Adjlist head of src vertice */
    graph->array[src].head = newNode;                             /* replace pointer to Adjlist of src with new Adjlist */

    newNode = newAdjListNode(src, weight);    /* allocs new Adjlist node with given src and weight */
    newNode->next = graph->array[dest].head;  /* points new Adjlist node to Adjlist head of dest vertice */
    graph->array[dest].head = newNode;        /* replace pointer to Adjlist of dest with new Adjlist */
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){ /* swaps given heap nodes a and b */
    struct MinHeapNode* t = *a; /* aux pointer to enable swap */
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx){  /* recursive heapify's heap from given point */
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if (smallest != idx)
    {
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap){ /* checks if heap is empty */
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap){  /* extracts the lowest value of the heap */
    if (isEmpty(minHeap)) /* checks if heap is empty */
        return NULL;

    struct MinHeapNode* root = minHeap->array[0]; /* finds root of heap */

    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1]; /* finds last node of heap */
    minHeap->array[0] = lastNode; /* puts last node of heap on top of the heap */

    minHeap->pos[root->v] = minHeap->size-1;  /* changes the location of root to end of heap */
    minHeap->pos[lastNode->v] = 0;            /* changes the location of last node to end start of heap */

    --minHeap->size;        /* decrease size of heap */
    minHeapify(minHeap, 0); /* heapify's heap */

    return root;  /* return extracted root */
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist){ /* change value of given edge in the heap to dist */
    int i = minHeap->pos[v];

    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v){ /* verify if given edge is in heap */
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

void dijkstra(struct Graph* graph, int src, int dest, problemsol **sol, char **wcache, int order, int maxmut){ /* performs the dijkstra algorithm in graph with src being the pos of first word and dest being the pos of second word also takes sol to store calculated path and weight, wcache to calculate paths based on indexes, problem order store solution in right order and problem maxweight to only use edges that don't violate that value */
    int V = graph->V;                           /* stores graph number of vertices */
    int v = 0;                                  /* aux value to help go through Adjlists */
    int dist[V];                                /* accumulated weight from src to all other vertices */
    struct MinHeap* minHeap = createMinHeap(V); /* creates new heap with size of number of vertices */
    struct MinHeapNode* auxHeapNode = NULL;     /* aux pointer */
    int parent[V];                              /* creates parent array to calculate best path */
    int aux = 0;                                /* to help go through parent array */
    list *path = NULL;                          /* word stack to store calculated path */

    maxmut++;                                   /* increases maxmut to help with comparison */

    for (v = 0; v < V; ++v) /* inits values */
    {
        dist[v] = INT_MAX;                              /* inits all dist as maximum possible int */
        parent[v] = -1;                                 /* inits parent values as -1 */
        minHeap->array[v] = newMinHeapNode(v, dist[v]); /* allocs all heap nodes */
        minHeap->pos[v] = v;                            /* inits pos in heap of v as v */
    }

    free(minHeap->array[src]);                            /* free alloced heap node for src */
    minHeap->array[src] = newMinHeapNode(src, dist[src]); /* alloc new heap node for src */
    minHeap->pos[src]   = src;                            /* init pos in heap of src as src */
    dist[src] = 0;                                        /* init distance from src to src as 0 */
    decreaseKey(minHeap, src, dist[src]);                 /* change src value in heap */

    minHeap->size = V;  /* init size of heap as number of vertices */

    while (!isEmpty(minHeap)) /* while heap is not empty */
    {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);  /* extracs min value */
        int u = minHeapNode->v;                                 /* stores index of vertice */

        struct AdjListNode* pCrawl = graph->array[u].head;      /* aux pointer to go through Adjlist */
        while (pCrawl != NULL)  /* while pCrawl is not NULL */
        {
            int v = pCrawl->dest; /* store destination */

            /* if dest is in heap and dist isn't max value of int and new weight is smaller than stored weight and that edge weight respects maxmut condition */
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                                          pCrawl->weight + dist[u] < dist[v] && pCrawl->weight < maxmut)
            {
                dist[v] = dist[u] + pCrawl->weight; /* update weight */

                parent[v] = u;  /* store parent vertice */

                decreaseKey(minHeap, v, dist[v]); /* change v value in heap */
            }
            pCrawl = pCrawl->next;  /* go to next Adjlist node */
        }

        free(minHeapNode);  /* free heap node */

        if(u == dest){                    /* present node is dest */
          if(dist[u] != INT_MAX){         /* if there is a solution */
            sol[order]->weight = dist[u]; /* store weight solution in respective solution */
          }
          else{                       /* there is no solution */
            sol[order]->weight = -1;  /* store weight as -1 to indicate no solution */
          }
          break;  /* breaks search */
        }
    }

    if(dist[dest] != INT_MAX){              /* if there is a solution */
      aux = dest;                           /* store dest in aux */
      while(aux != src){                    /* while aux isn't src */
        path = newlist(wcache[aux], path);  /* push word to path stack */
        aux = parent[aux];                  /* go to parent of current vertice */
      }
    }
    else{ /* if there is no solution */
      path = newlist(wcache[dest], path); /* push dest word to path stack */
    }

    path = newlist(wcache[src], path);  /* push src word to path stack */

    sol[order]->path = path;  /* store path in respective solution path */

    while(!isEmpty(minHeap)){ /* free rest of heap */
      auxHeapNode = extractMin(minHeap);
      free(auxHeapNode);
    }

    free(minHeap->pos);   /* free heap pos array */
    free(minHeap->array); /* free heap array of heap nodes */
    free(minHeap);        /* free heap */
}

void freegraph(struct Graph* graph, int V){ /* free given graph */
  int counter = 0;                  /* counter to help go through graph arrays */
  struct AdjListNode* aux = NULL;   /* aux pointer to help free AdjLists */
  struct AdjListNode* head = NULL;  /* head pointer to help free AdjLists */

  for(counter = 0; counter < V; counter++){ /* goes through graph multiple AdjLists */
    head = graph->array[counter].head;      /* gets pointer to head of AdjLists */
    while(head != NULL){                    /* while AdjList is not empty */
      aux = head->next;                     /* store pointer to next */
      free(head);                           /* free current head */
      head = aux;                           /* point head to next */
    }
  }

  free(graph->array); /* free array of AdjLists */
  free(graph);        /* free graph */
}
