#ifndef HEAP_H
#define HEAP_H

typedef struct Entry {
	int priority;
	int x;
	int y;
} Entry;

typedef struct Heap {
	Entry* array;
	int size;
	int front;
} Heap;

#include <stdlib.h>
#include <stdio.h>

/* Code for a min-priority heap structure */

void swap(Entry* a, Entry* b);
Entry newEntry(int priority, int x, int y);
Heap newEmptyHeap();
void freeHeap(Heap h);
int isEmpty(Heap* h);
int doubleHeapSize(Heap* h);
void upheap(Heap* h, int childIdx);
void downheap(Heap *h, int idx);
void enqueue(Heap* h, Entry n);
Entry removeMin(Heap *h);

#endif