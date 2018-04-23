#ifndef FRINGE_H
#define FRINGE_H
#include <stdarg.h>

#include "state.h"

#define MAXF 500000  /* maximum fringe size */

#define LIFO  1
#define STACK 2
#define FIFO  3
#define PRIO  4
#define HEAP  5

typedef struct Fringe {
  int mode;      /* can be LIFO(STACK), FIFO, or PRIO(HEAP)          */
  int size;      /* number of elements in the fringe                 */
  int front;     /* index of first element in the fringe (FIFO mode) */
  int rear;      /* index of last element in the fringe (FIFO mode)  */
  State *states; /* fringe data (states)                             */
  int insertCnt; /* counts the number of insertions                  */
  int deleteCnt; /* counts the number of removals (deletions)        */
  int maxSize;   /* maximum size of the fringe during search         */
} Fringe;

Fringe makeFringe(int mode);
/* Returns an empty fringe. 
 * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP) 
 */

void deallocFringe(Fringe fringe);
/* Frees the memory allocated for the fringe */

int getFringeSize(Fringe fringe);
/* Returns the number of elements in the fringe 
 */

int isEmptyFringe(Fringe fringe);
/* Returns 1 if the fringe is empty, otherwise 0 */

Fringe insertFringe(Fringe fringe, State s, ...);
/* Inserts s in the fringe, and returns the new fringe.
 * This function needs a third parameter in PRIO(HEAP) mode.
 */

Fringe removeFringe(Fringe fringe, State *s);
/* Removes an element from the fringe, and returns it in s. 
 * Moreover, the new fringe is returned.
 */

void showStats(Fringe fringe);
/* Shows fringe statistics */

#endif
