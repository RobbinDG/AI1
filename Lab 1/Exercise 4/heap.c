#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

/* Code for a min-priority heap structure */

void swap(Entry* a, Entry* b){
	Entry t = *a;
	*a = *b;
	*b = t;
}

Entry newEntry(int priority, int x, int y){
	Entry e;
	e.priority = priority;
	e.x = x;
	e.y = y;
	return e;
}

Heap newEmptyHeap(){
	Heap h;
	// indices start at 1, first array element is left undefined
	h.array = malloc(2 * sizeof(Entry));
	h.size = 2;
	h.front = 1;
	return h;
}

void freeHeap(Heap h){
	free(h.array);
}

int isEmpty(Heap* h){
	return (h->front == 0);
}

int doubleHeapSize(Heap* h){
	int newSize = 2 * h->size;
	h->array = realloc(h->array, newSize * sizeof(Entry));
	if(h->array == NULL) fprintf(stderr, "Fatal error: realloc() failed\n");
	return newSize;
}

void upheap(Heap* h, int childIdx){
	if(childIdx == 1) return;
	int parentIdx = childIdx/2;
	if(h->array[childIdx].priority < h->array[parentIdx].priority){
		swap(&(h->array[childIdx]), &(h->array[parentIdx]));
		upheap(h, parentIdx);
	}
}

void downheap(Heap *h, int idx){
	if(2*idx < h->front){
		Entry* lc = h->array + 2*idx;
		Entry* rc = (2*idx+1 < h->front ? h->array + 2*idx+1 : lc);
		if(h->array[idx].priority > lc->priority && lc->priority <= rc->priority){
			swap(lc, &(h->array[idx]));
			downheap(h, 2*idx);
		} else if (h->array[idx].priority > rc->priority){
			swap(rc, &(h->array[idx]));
			downheap(h, 2*idx+1);
		}
	}
}

void enqueue(Heap* h, Entry n){
	if(h->size == h->front) h->size = doubleHeapSize(h);
	h->array[h->front] = n;
	upheap(h, (h->front)++);
}

Entry removeMin(Heap *h){
	Entry r = h->array[1];
	h->array[1] = h->array[--(h->front)];
	downheap(h, 1);
	return r;
}