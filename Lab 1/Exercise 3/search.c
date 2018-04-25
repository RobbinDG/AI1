#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"
#include "searchTree.h"

#define RANGE 1000000

Fringe insertValidSucc(Fringe fringe, int value, Tree explored) {
  State s;
  if (isInSearchTree (explored, value) || (value < 0) || (value > RANGE)) {
    /* ignore states that are out of bounds */
    return fringe;
  }
  s.value = value;
  return insertFringe(fringe, s);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value;
  Tree explored = emptyTree();
  
  fringe = makeFringe(mode);
  state.value = start;
  fringe = insertFringe(fringe, state);
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited++;
    /* is state the goal? */
    value = state.value;
    if (value == goal) {
      goalReached = 1;
      break;
    }
	explored = addInSearchTree (explored, value);

    /* insert neighbouring states */
    fringe = insertValidSucc(fringe, value+1, explored); /* rule n->n + 1      */
    fringe = insertValidSucc(fringe, value-1, explored); /* rule n->n - 1      */
	if (value)
	{
		fringe = insertValidSucc(fringe, 2*value, explored); /* rule n->2*n        */
		fringe = insertValidSucc(fringe, 3*value, explored); /* rule n->3*n        */
		fringe = insertValidSucc(fringe, value/2, explored); /* rule n->floor(n/2) */
		fringe = insertValidSucc(fringe, value/3, explored); /* rule n->floor(n/3) */
	}
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);
  freeSearchTree(explored);
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }

  start = 0;
  goal = 42;
  if (argc == 3) {
    goal = atoi(argv[2]);
  } else if (argc == 4) {
    start = atoi(argv[2]);
    goal = atoi(argv[3]);
  }

  printf("Problem: route from %d to %d\n", start, goal);
  search(fringetype, start, goal); 
  return EXIT_SUCCESS;
}