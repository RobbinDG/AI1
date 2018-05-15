#include <stdio.h>
#include <stdlib.h>

#define DEBUG 0

#define MAX 0
#define MIN 1

#define INFINITY 9999999
#define MAXI(a,b) (((a)>(b))?(a):(b))

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
	  #if DEBUG
			printf("++m: %d\n", max);
		#endif
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
	  #if DEBUG
		printf("--m: %d\n", min);
	  #endif
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
		// #if DEBUG
			// printf("++m: %d\n", m);
		// #endif
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
	  // #if DEBUG
		// printf("--m: %d\n", m);
	  // #endif
      if (m < min) {
        min = m;
        bestmove = move;
      }
    }
  }
  return bestmove;
}

int* negaMax (int state, int colour, int pair[2], int transpositionTable[][3]) {
	if (transpositionTable[state][1] != 0) {
		pair[0] = transpositionTable[state][0];
		if (transpositionTable[state][2] == colour)
			pair[1] = transpositionTable[state][1];
		else 
			pair[1] = -transpositionTable[state][1];
		return pair;
	}
	int move, bestmove, m, min = -colour*INFINITY;
	/* terminal state ? */
	if (state == 1) {
		pair[0] = 0;
		pair[1] = -colour;
		return pair; /* Max wins if min is in a terminal state */
		// return -colour;
	} 
	/* non-terminal state */  
	for (move = 1; move <= 3; move++) {
		if (state - move > 0) { /* legal move */
			pair = negaMax(state-move, -colour, pair, transpositionTable);
			m = pair[1];
			if (colour*m > colour*min) {
				min = m;
				bestmove = move;
			}
			#if DEBUG
				printf("%d%dm: %d\n", colour, colour, min);
			#endif
		}
	}
	transpositionTable[state][0] = pair[0] = bestmove;
	transpositionTable[state][1] = pair[1] = min;
	transpositionTable[state][2] = colour;
	return pair;
}

int minimaxDecisionNega(int state, int turn, int transpositionTable[][3]) {
  int pair[2];
  if (turn == MAX) {
	negaMax(state, 1, pair, transpositionTable);
    return pair[0];
  }
  /* turn == MIN */
  negaMax(state, -1, pair, transpositionTable);
  return pair[0];
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

void playNimNega(int state, int transpositionTable[][3]) {
  int turn = 0;
  while (state != 1) {
    int action = minimaxDecisionNega(state, turn, transpositionTable);
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
  
  int transpositionTable[101][3] = {{0}};
  
  playNim(atoi(argv[1]));
  printf("======\n");  
  playNimNega(atoi(argv[1]), transpositionTable);

  return 0;
}
