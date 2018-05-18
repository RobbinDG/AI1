#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

int minValue(int state); /* forward declaration: mutual recursion */

int maxValue(int state) {
  int move, max = -INFINITY;
  /* terminal state ? */
  if (state == 1) {
    return -1; /* Min wins if max is in a terminal state */
  }
  /* non-terminal state */
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = minValue(state - move);
      if (m > max) max = m;
    }
  }
  return max;
}

int minValue(int state) {
  int move, min = INFINITY;
  /* terminal state ? */
  if (state == 1) {
    return 1; /* Max wins if min is in a terminal state */
  } 
  /* non-terminal state */  
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = maxValue(state - move);
      if (m < min) min = m;
    }
  }
  return min;
}

int minimaxDecision(int state, int turn) {
  int move, bestmove, max, min;
  if (turn == MAX) {
    max = -INFINITY;
    for (move = 1; move <= 3; move++) {
      if (state - move > 0) { /* legal move */
        int m = minValue(state - move);
        if (m > max) {
          max = m;
          bestmove = move;
        }
      }
    }
    return bestmove;
  }
  /* turn == MIN */
  min = INFINITY;
  for (move = 1; move <= 3; move++) {
    if (state - move > 0) { /* legal move */
      int m = maxValue(state - move);
      if (m < min) {
        min = m;
        bestmove = move;
      }
    }
  }
  return bestmove;
}

void playNim(int state) {
  int turn = 0;
  while (state != 1) {
    int action = minimaxDecision(state, turn);
    printf("%d: %s takes %d\n", state, 
           (turn==MAX ? "Max" : "Min"), action);
    state = state - action;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
	int numSticks;
	do {
		printf ("Number of sticks (3<=sticks<=100): ");
		if(argc == 3) 
			numSticks = atoi(argv[1]);
		else
			scanf ("%d", &numSticks);
	} while ((numSticks < 3) || (numSticks > 100));
	
	int algorithm;
	do {
		printf ("Algorithm: (1) MinMaxDecisions  (2) NegaMax  (3) Transposition table");
		if(argc == 3) 
			algorithm = atoi(argv[2]);
		else
			scanf ("%d", &algorithm);
	} while ((algorithm < 1) || (algorithm > 3));
	
	switch (algorithm) {
		case 1: 
	}
  // if ((argc != 2) || (atoi(argv[1]) < 3)) {
    // fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    // fprintf(stderr, "<number of sticks> must be at least 3!\n");
    // return -1;
  // }

  playNim(atoi(argv[1]));

  return 0;
}