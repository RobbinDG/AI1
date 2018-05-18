#include <stdio.h>
#include <stdlib.h>

#define MAX 0
#define MIN 1

#define INFINITY 9999999

int* negaMax (int state, int colour, int pair[2]) {
	int move, bestmove, minMax = -colour*INFINITY;
	/* terminal state ? */
	if (state == 1) {
		pair[1] = -colour; /* Max wins if min is in a terminal state */
		return pair; 
	} 
	/* non-terminal state */  
	for (move = 1; move <= 3; move++) {
		if (state - move > 0) { /* legal move */
			pair = negaMax(state-move, -colour, pair);
			if (colour*pair[1] > colour*minMax) {
				minMax = pair[1];
				bestmove = move;
			}
		}
	}
	pair[0] = bestmove;
	pair[1] = minMax;
	return pair;
}

int minimaxDecisionNegaMax(int state, int turn) {
  int pair[2];
  if (turn == MAX) {
	negaMax(state, 1, pair);
    return pair[0];
  }
  /* turn == MIN */
  negaMax(state, -1, pair);
  return pair[0];
}

void playNim(int state) {
  int turn = 0;
  while (state != 1) {
    int action = minimaxDecisionNegaMax(state, turn);
    printf("%d: %s takes %d\n", state, 
           (turn==MAX ? "Max" : "Min"), action);
    state = state - action;
    turn = 1 - turn;
  }
  printf("1: %s looses\n", (turn==MAX ? "Max" : "Min"));
}

int main(int argc, char *argv[]) {
  if ((argc != 2) || (atoi(argv[1]) < 3)) {
    fprintf(stderr, "Usage: %s <number of sticks>, where ", argv[0]);
    fprintf(stderr, "<number of sticks> must be at least 3!\n");
    return -1;
  }
  
  playNim(atoi(argv[1]));

  return 0;
}
