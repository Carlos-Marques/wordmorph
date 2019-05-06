#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "list.h"

struct AdjListNode{
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList{
    struct AdjListNode *head;
};

struct Graph{
    int V;
    struct AdjList* array;
};

struct MinHeapNode{
    int  v;
    int dist;
};

struct MinHeap{
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct AdjListNode* newAdjListNode(int dest, int weight); /* creates new node for adjlist with given dest and weight */
struct Graph* createGraph(int V); /* creates graph based on number of vertices given */
struct MinHeapNode* newMinHeapNode(int v, int dist);  /* adds new node to heap with destination v and weight dist */
struct MinHeap* createMinHeap(int capacity);  /* creates heap of given capacity */
void addEdge(struct Graph* graph, int src, int dest, int weight); /* adds edge to given graph with src, dest and weight */
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b); /* swaps given heap nodes a and b */
void minHeapify(struct MinHeap* minHeap, int idx);  /* recursive heapify's heap from given point */
int isEmpty(struct MinHeap* minHeap); /* checks if heap is empty */
struct MinHeapNode* extractMin(struct MinHeap* minHeap);  /* extracts the lowest value of the heap */
void decreaseKey(struct MinHeap* minHeap, int v, int dist); /* change value of given edge in the heap to dist */
bool isInMinHeap(struct MinHeap *minHeap, int v); /* verify if given edge is in heap */
void dijkstra(struct Graph* graph, int src, int dest, problemsol **sol, char **wcache, int order, int maxmut);  /* performs the dijkstra algorithm in graph with src being the pos of first word and dest being the pos of second word also takes sol to store calculated path and weight, wcache to calculate paths based on indexes, problem order store solution in right order and problem maxweight to only use edges that don't violate that value */
void freegraph(struct Graph* graph, int V); /* free given graph */

#endif
