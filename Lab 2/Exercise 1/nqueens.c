/* nqueens.c: (c) Arnold Meijster (a.meijster@rug.nl) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXQ 100

#define MAXGEN 5000
#define GENSIZE 50

#define TESTING_MODE 0

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens: global variable */
int queens[MAXQ]; /* queen at (r,c) is represented by queens[r] == c */

void initializeRandomGenerator() {
  /* this routine initializes the random generator. You are not
   * supposed to understand this code. You can simply use it.
   */
  time_t t;
  srand((unsigned) time(&t));
}

/* Generate an initial position.
 * If flag == 0, then for each row, a queen is placed in the first column.
 * If flag == 1, then for each row, a queen is placed in a random column.
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    queens[q] = (flag == 0 ? 0 : random() % nqueens); 
  }
}

/* returns TRUE if position (row0,column0) is in 
 * conflict with (row1,column1), otherwise FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /* on same row, */
  if (column0 == column1) return TRUE; /* column, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonal */
  return FALSE; /* no conflict */
}

/* returns TRUE if position (row,col) is in 
 * conflict with any other queen on the board, otherwise FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, queens[queen])) {
      if ((row != queen) || (col != queens[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print configuration on screen */
void printState() {
  if(TESTING_MODE) return;
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (queens[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}

/* move queen on row q to specified column, i.e. to (q,column) */
void moveQueen(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if ((column < 0) || (column >= nqueens)) {
    fprintf(stderr, "Error in moveQueen: column=%d "
      "(should be 0<=column<%d)...Abort.\n", column, nqueens);
    exit(-1);
  }
  queens[queen] = column;
}

/* returns TRUE if queen can be moved to position
 * (queen,column). Note that this routine checks only that 
 * the values of queen and column are valid! It does not test
 * conflicts!
 */
int canMoveTo(int queen, int column) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in canMoveTo: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  if(column < 0 || column >= nqueens) return FALSE;
  if (queens[queen] == column) return FALSE; /* queen already there */
  return TRUE;
}

/* returns the column number of the specified queen */
int columnOfQueen(int queen) {
  if ((queen < 0) || (queen >= nqueens)) {
    fprintf(stderr, "Error in columnOfQueen: queen=%d "
      "(should be 0<=queen<%d)...Abort.\n", queen, nqueens);
    exit(-1);
  }
  return queens[queen];
}

/* returns the number of pairs of queens that are in conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, queens[queen], other, queens[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. The maximal number of queens in conflict
 * can be 1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * Since we want to do ascending local searches, the evaluation
 * function returns (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

/*************************************************************/

/* A very silly random search 'algorithm' */
#define MAXITER 1000
void randomSearch() {
  int queen, iter = 0;
  int optimum = (nqueens-1)*nqueens/2;

  while (evaluateState() != optimum) {
    printf("iteration %d: evaluation=%d\n", iter++, evaluateState());
    if (iter == MAXITER) break;  /* give up */
    /* generate a (new) random state: for each queen do ...*/
    for (queen=0; queen < nqueens; queen++) {
      int pos, newpos;
      /* position (=column) of queen */
      pos = columnOfQueen(queen);
      /* change in random new location */
      newpos = pos;
      while (newpos == pos) {
        newpos = random() % nqueens;
      }
      moveQueen(queen, newpos);
    }
  }
  if (iter < MAXITER) {
    printf ("Solved puzzle. ");
  }
  printf ("Final state is");
  printState();
}

/*************************************************************/

int sharesColumnWithPreviousQueen(int row, int column){
	for(int i = 0; i < row; ++i){
		if(queens[i] == column) return TRUE;
	}
	return FALSE;
}

void hillClimbing() {
  int statelist[MAXQ];
  for(int i = 0; i < nqueens; ++i) {
	  /* store state ranks in array statelist */
	  for(int j = 0; j < nqueens; ++j) {
		  if(!sharesColumnWithPreviousQueen(i,j)) {
			  queens[i] = j;
			  statelist[j] = evaluateState();
		  } else {
			  statelist[j] = -1;
		  }
	  }
	  
	  /* find amount of maximal ranks */
	  int maxVal = -1, maxCnt = 0;
	  for(int j = 0; j < nqueens; ++j) {
		  if(statelist[j] == maxVal) ++maxCnt;
		  if(statelist[j] > maxVal) {
			  maxVal = statelist[j];
			  maxCnt = 1;
		  }
	  }
	  
	  /* determine a random position from the options
	   * and move the queen there */
	   if(maxCnt == 0) continue;
	   int choice = random() % maxCnt;
	   int cnt = 0;
	   for(int j = 0; j < nqueens; ++j) {
		   if(statelist[j] == maxVal){
			   if(cnt == choice){
				   queens[i] = j;
				   break;
			   }
			   ++cnt;
		   }
	   }
	}
	if(!TESTING_MODE) printf("\nFinal State");
	printState();
}

void randomRestartHillClimbing() {
    int maxRestarts = 25;
    int best[MAXQ];
    int bestRank = 0;
    for(int i = 0; i < maxRestarts; ++i) {
        int statelist[MAXQ];
        initiateQueens(1);
        for(int i = 0; i < nqueens; ++i) {
            /* store state ranks in array statelist */
            for(int j = 0; j < nqueens; ++j) {
              if(!sharesColumnWithPreviousQueen(i,j)) {
                  queens[i] = j;
                  statelist[j] = evaluateState();
              } else {
                  statelist[j] = -1;
              }
            }

            /* find amount of maximal ranks */
            int maxVal = -1, maxCnt = 0;
            for(int j = 0; j < nqueens; ++j) {
                if(statelist[j] == maxVal) ++maxCnt;
                if(statelist[j] > maxVal) {
                    maxVal = statelist[j];
                    maxCnt = 1;
                }
            }

            /* determine a random position from the options
            * and move the queen there */
            if(maxCnt == 0) continue;
            int choice = random() % maxCnt;
            int cnt = 0;
            for(int j = 0; j < nqueens; ++j) {
                if(statelist[j] == maxVal){
                    if(cnt == choice){
                        queens[i] = j;
                        break;
                    }
                    ++cnt;
                }
            }
        }
        int rank;
        if((rank = evaluateState()) > bestRank) {
            for(int i = 0; i < nqueens; ++i) {
                best[i] = queens[i];
            }
            bestRank = rank;
        }
    }
    for(int i = 0; i < nqueens; ++i) {
        queens[i] = best[i];
    }
    if(!TESTING_MODE) printf("\nFinal State");
    printState();
}

/*************************************************************/

void simulatedAnnealing() {
  printf("Implement the routine simulatedAnnealing() yourself!!\n");
}

/*************************************************************/

void printChromosome(int* chrom){
    for(int i = 0; i < nqueens; ++i) {
        queens[i] = chrom[i];
    }
    printState();
}

void freeGeneration(int** chromosomes) {
    for(int i = 0; i < nqueens; ++i) {
        free(chromosomes[i]);
    }
    free(chromosomes);
}

void initializeGeneration(int** generation) {
    for(int i = 0; i < GENSIZE; ++i) { 
        for(int j = 0; j < nqueens; ++j) {
            generation[i][j] = random() % nqueens;
        }
    }
}

int calcFitness(int* chrom) {
    for(int i = 0; i < nqueens; ++i) {
        queens[i] = chrom[i];
    }
    return evaluateState();
}

int getFittestIndex(int** chromosomes) {
    int idx = 0, fitness = calcFitness(chromosomes[0]);
    for(int i = 1; i < GENSIZE; ++i) {
        int curFitness = calcFitness(chromosomes[i]);
        if(curFitness > fitness) {
            idx = i;
            fitness = curFitness;
        }
    }
    return idx;
}

/* Mutates one gene in 1/4 new chromosomes
 */
void mutate(int* chrom){
    if(!random() % 4) {
        chrom[random() % nqueens] = random() % nqueens;
    }
}

int* crossover(int** a, int** b){
    int cutPoint = random() % nqueens;
    int* new = malloc(nqueens * sizeof(int));
    for(int i = 0; i < nqueens; ++i) {
        if(i < cutPoint) {
            new[i] = (*a)[i];
        } else {
            new[i] = (*b)[i];
        }
    }
    return new;
}

/* returns a new generation of chromosomes */
int** reproduce(int** chromosomes, int generationIdx){
    int** newGeneration = malloc(GENSIZE * sizeof(int*));
    for(int i = 0; i < GENSIZE; ++i) {
        newGeneration[i] = malloc(nqueens * sizeof(int));
    }

    int* fitnesses = malloc(GENSIZE * sizeof(int));
    int totalFitness = 0;
    for(int i = 0; i < GENSIZE; ++i) {
        fitnesses[i] = calcFitness(chromosomes[i]);
        totalFitness += fitnesses[i];
    }

    for(int i = 0; i < GENSIZE; ++i) {
        int cr1 = random() % totalFitness;
        int cr2 = random() % totalFitness;
        int a = 0, b = 0;
        for(int j = 0; j < GENSIZE; ++j) {
            if(cr1 < fitnesses[j] && cr1 >= 0) a = j;
            if(cr2 < fitnesses[j] && cr2 >= 0) b = j;
            cr1 -= fitnesses[j];
            cr2 -= fitnesses[j];
        }
        // preventing inbreeding
        if(a == b) {
            --i; continue;
        }
        newGeneration[i] = crossover(&chromosomes[a], &chromosomes[b]);
        mutate(chromosomes[i]);
    }

    free(fitnesses);
    freeGeneration(chromosomes);
    return newGeneration;
}

void geneticPermutation(){
    int** chromosomes = malloc(GENSIZE * sizeof(int*));
    for(int i = 0; i < GENSIZE; ++i) { 
        chromosomes[i] = malloc(nqueens * sizeof(int));
    }
    initializeGeneration(chromosomes);

    int maxFitness = (nqueens-1)*nqueens/2;
    for(int i = 0; i < MAXGEN; ++i) {
        chromosomes = reproduce(chromosomes, i);
        int max = getFittestIndex(chromosomes);
        if(calcFitness(chromosomes[max]) == maxFitness) {
            printChromosome(chromosomes[max]);
            return;
        }
    }
    int fittest = getFittestIndex(chromosomes);
    printf("terminated with %d/%d\n", calcFitness(chromosomes[fittest]), maxFitness);
    printChromosome(chromosomes[fittest]);

    freeGeneration(chromosomes);
}


int main(int argc, char *argv[]) {
  int algorithm;

  do {
    if(!TESTING_MODE) printf ("Number of queens (1<=nqueens<%d): ", MAXQ);
    if(argc == 3) 
        nqueens = atoi(argv[1]);
    else
        scanf ("%d", &nqueens);
  } while ((nqueens < 1) || (nqueens > MAXQ));

  do {
    if(!TESTING_MODE) {
        printf ("Algorithm: (1) Random search  (2) Hill climbing  ");
        printf ("(3) Simulated Annealing  (4) Genetic Permutation: ");
    }
    if(argc == 3) 
        algorithm = atoi(argv[2]);
    else
        scanf ("%d", &algorithm);
  } while ((algorithm < 1) || (algorithm > 4));
  
  initializeRandomGenerator();

  initiateQueens(1);
  
  if(!TESTING_MODE) printf("\nInitial state:");
  printState();

  switch (algorithm) {
  case 1: randomSearch();              break;
  case 2: randomRestartHillClimbing(); break;
  case 3: simulatedAnnealing();        break;
  case 4: geneticPermutation();        break;
  }

  if(TESTING_MODE) {
    FILE *out = fopen("1.out", "a");  
    fprintf(out, "{%.1f,%d}", (double)nqueens/10, countConflicts());  
    fclose(out);
  }
  return 0;  
}
