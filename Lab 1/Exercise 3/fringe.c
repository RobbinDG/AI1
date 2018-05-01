#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fringe.h"

Fringe makeFringe(int mode) {
  /* Returns an empty fringe. 
   * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP) 
   */
  Fringe f;
  if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
      (mode != PRIO) && (mode != HEAP)) {
    fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
    fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
    exit(EXIT_FAILURE);
  }
  f.mode = mode;
  if (f.mode == PRIO || f.mode == HEAP) {
	  f.size = f.front = 1;
  } else {
	  f.size = f.front = 0;
  }
  f.rear = 0;      /* front+rear only used in FIFO mode */
  f.states = malloc(MAXF*sizeof(State));
  if (f.states == NULL) {
	fprintf(stderr, "makeFringe(): memory allocation failed.\n");
    exit(EXIT_FAILURE);      
  }
  f.maxSize = f.insertCnt = f.deleteCnt = 0;
  return f;
}

void deallocFringe(Fringe fringe) {
  /* Frees the memory allocated for the fringe */
  free(fringe.states);
}

int getFringeSize(Fringe fringe) {
  /* Returns the number of elements in the fringe 
   */
  return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
  /* Returns 1 if the fringe is empty, otherwise 0 */
  return (fringe.size == 0 ? 1 : 0);
}

Fringe insertFringe(Fringe fringe, State s, ...) {
  /* Inserts s in the fringe, and returns the new fringe.
   * This function needs a third parameter in PRIO(HEAP) mode.
   */
  int fr;

  if (fringe.size == MAXF) {
    fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.insertCnt++;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
    fringe.states[fringe.size] = s;
    break;
  case FIFO:
    fringe.states[fringe.rear++] = s;
    fringe.rear %= MAXF;
    break;
  case PRIO: /* PRIO == HEAP */
  case HEAP:
    fr = fringe.front;
	fringe.states[fr] = s;
	upHeap (&fringe, fr);
	fringe.front++;
    break;
  }
  fringe.size++;
  if (fringe.size > fringe.maxSize) {
    fringe.maxSize = fringe.size;
  }
  return fringe;
}

Fringe removeFringe(Fringe fringe, State *s) {
  /* Removes an element from the fringe, and returns it in s. 
   * Moreover, the new fringe is returned.
   */
  if (fringe.size < 1) {
    fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
    exit(EXIT_FAILURE);    
  }
  fringe.deleteCnt++;
  fringe.size--;
  switch (fringe.mode) {
  case LIFO: /* LIFO == STACK */
  case STACK:
    *s = fringe.states[fringe.size];
    break;
  case FIFO:
    *s = fringe.states[fringe.front++];
    fringe.front %= MAXF;
    break;
  case PRIO: /* PRIO == HEAP */
  case HEAP:
	*s = fringe.states[1];
	fringe.states[1] = fringe.states[--(fringe.front)];
	downHeap (&fringe, 1);
    break;
  }
  return fringe;
}

void upHeap(Fringe* f, int childIdx){
	if(childIdx == 1) return;
	int parentIdx = childIdx/2;
	if(f->states[childIdx].cost < f->states[parentIdx].cost){
		swap(&(f->states[childIdx]), &(f->states[parentIdx]));
		upHeap(f, parentIdx);
	}
}

void downHeap(Fringe *f, int idx){
	if(2*idx < f->front){
		State* lc = &f->states[2*idx];
		State* rc = (2*idx+1 < f->front ? &(f->states[2*idx+1]) : lc);
		if(f->states[idx].cost > lc->cost && lc->cost <= rc->cost){
			swap(lc, &(f->states[idx]));
			downHeap(f, 2*idx);
		} else if (f->states[idx].cost > rc->cost){
			swap(rc, &(f->states[idx]));
			downHeap(f, 2*idx+1);
		}
	}
}

void swap (State *a, State *b) {
	State c = *a;
	*a = *b;
	*b = c;
}

void showStats(Fringe fringe) {
  /* Shows fringe statistics */
  printf("#### fringe statistics:\n");
  printf(" #size        : %7d\n", fringe.size);
  printf(" #maximum size: %7d\n", fringe.maxSize);
  printf(" #insertions  : %7d\n", fringe.insertCnt);
  printf(" #deletions   : %7d\n", fringe.deleteCnt);
  printf("####\n"); 
}
