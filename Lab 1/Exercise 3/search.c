#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"
#include "searchTree.h"

#define RANGE 1000000

void printPathRec (Tree t, int *length) {
	if (t->state.pathParent == NULL) {
		printf("%d ", t->state.value);
		return;
	}
	(*length)++;
	printPathRec(t->state.pathParent, length);
	switch(t->state.action){
      case PLUS1:
        printf("(+1)-> ");
        break;
      case MIN1:
        printf("(-1)-> ");
        break;
      case TIMES2:
        printf("(*2)-> ");
        break;
      case TIMES3:
        printf("(*3)-> ");
        break;
      case DIV2:
        printf("(/2)-> ");
        break;
      case DIV3:
        printf("(/3)-> ");
        break;
      default:
        printf("ERROR IS SWITCH/CASE\n");
        break;
    }
	printf("%d ", t->state.value);
}

void printPath (Tree t) {
	int length = 0;
	printPathRec(t, &length);
	printf ("\nlength = %d, cost = %d\n", length, t->state.cost);
}

Fringe insertValidSucc(Fringe fringe, Tree explored, Tree pathParent, int value, int cost, int action, int depth) {
  State s;
  // Second modification
  if (isInSearchTreeN (explored, value) || (value < 0) || (value > RANGE) ) {
    /* ignore states that are already explored or out of bounds */
    return fringe;
  }

  s.value = value;
  s.cost = cost;
  s.action = action;
  s.depth = depth;
  s.pathParent = pathParent;
  return insertFringe(fringe, s);
}

void search(int mode, int start, int goal) {
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value = 0;
  Tree explored = emptyTree();
  Tree new = emptyTree();
  
  fringe = makeFringe(mode);
  int maxDepth;
  if (fringe.mode == IDS) {
	  maxDepth = abs(goal - start);
  } else {
	  /* Setting maxDepth to 1 means the outer loop will run only once, and thus nothing
	  has changed for the other algortihms */
	  maxDepth = 1; 
  }

  int depth = 0;
  /* Loop and increment depth until we have reached maxDepth (or reached a goal state) */
  while (goalReached != 1 && depth < maxDepth) {
	  state.value = start;
      state.cost = 0;
      state.depth = 0;
      state.pathParent = emptyTree();
      freeSearchTree(explored);
      explored = emptyTree();
	  if (fringe.mode == PRIO || fringe.mode == HEAP) {
		  fringe.size = 1;
	  } else {
		  fringe.size = 0;
	  }
      fringe = insertFringe(fringe, state);
	  while (!isEmptyFringe(fringe)) {
		/* get a state from the fringe */
		fringe = removeFringe(fringe, &state);
		/* In insertValidSucc we check if value is already in explored. We do it again here, because sometimes (for example in
		UCS) we also want to check if a state isn't already in the fringe. In UCS we actually only add states to the fringe if
		it is not in explored or in the fringe. If it is in the fringe, we want the state with the lowest cost. We don't implement
		it like this; in insertValidSucc multiple states with the same value can be in the fringe before that value has been 
		explored, but since the fringe is a heap, the state with the lowest cost will be on top and thus the state with some 
		value will be the value with the lowest cost and all other states with that value ar ignored, which, in the end, is what we want*/
		if (isInSearchTreeN (explored, state.value)) // Second modification
			continue;
		if (fringe.mode == IDS && state.depth > depth) // For IDS, ignore states beyond depth
			continue;
		visited++;
		value = state.value;
		new = newTree (state, emptyTree(), emptyTree());
		explored = addNodeInSearchTree (explored, new);
		/* is state the goal? */
		if (value == goal) {
		  goalReached = 1;
		  break;
		}

		/* insert neighbouring states */
		fringe = insertValidSucc(fringe, explored, new, value+1, state.cost+1, PLUS1, state.depth+1); /* rule n->n + 1      */
		if (value != 0) // First modification
		{
		  fringe = insertValidSucc(fringe, explored, new, value-1, state.cost+1, MIN1, state.depth+1); /* rule n->n - 1      */
		  fringe = insertValidSucc(fringe, explored, new, 2*value, state.cost+2, TIMES2, state.depth+1); /* rule n->2*n        */
		  fringe = insertValidSucc(fringe, explored, new, 3*value, state.cost+2, TIMES3, state.depth+1); /* rule n->3*n        */
		  fringe = insertValidSucc(fringe, explored, new, value/2, state.cost+3, DIV2, state.depth+1); /* rule n->floor(n/2) */
		  fringe = insertValidSucc(fringe, explored, new, value/3, state.cost+3, DIV3, state.depth+1); /* rule n->floor(n/3) */
		}
	  }
	  depth++;	  
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  if (goalReached) printPath(new);
  deallocFringe(fringe);
  freeSearchTree(explored);
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
  if ((argc == 1) || (argc > 4)) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP|IDS> [start] [goal]\n", argv[0]);
    return EXIT_FAILURE;
  }
  fringetype = 0;
  
  if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
    fringetype = STACK;
  } else if (strcmp(argv[1], "FIFO") == 0) {
    fringetype = FIFO;
  } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
    fringetype = HEAP;
  } else if (strcmp(argv[1], "IDS") == 0) {
	  fringetype = IDS;
  }
  if (fringetype == 0) {
    fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP|IDS> [start] [goal]\n", argv[0]);
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