#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CNFify.h"

/* Assumption: the propositional symbols are a, b, .., z.
 * So, each propositional formula contains at most 26 
 * different variables. Sets of variables can therefore 
 * be represented by 26 bits, hence an integer (bitstring 
 * of 32 bits) suffices.
 */

#define FALSE 0
#define TRUE  1

typedef unsigned int bitset;

typedef struct clause {
  bitset positive;   /* bitset of positive symbols in clause */
  bitset negative;   /* bitset of negative symbols in clause */
} clause;

typedef struct clauseSet {
  int size;      /* number of clauses in set                    */
  int allocated; /* maximum number of clauses (allocated space) */
  clause *clauses;
} clauseSet;

/******** ADT for clauses ***************************************/

int isEmptyClause(clause c) {
  /* returns TRUE if and only if the clauses c is empty */
  return ((c.positive == 0) && (c.negative == 0) ? TRUE : FALSE);
}

void makeEmptyClause(clause *c) {
  /* makes c the empty clause (i.e. false) */
  c->positive = 0;
  c->negative = 0;
}

int expectLetter(char c) {
  /* helper routine for parsing clause strings in makeClause() */
  if ((c < 'a') || (c > 'z')) {
    fprintf(stderr, "Syntax error: expected a letter ('a'..'z')\n");
    exit(EXIT_FAILURE);
  }
  return c - 'a';
}

void makeClause(clause *c, char *cstr) {
  /* Converts the string cstr into a clause.
   * For example, the clause {a,b,~c} is represented 
   * by the string "a,b,~c".
   */
  int n, idx = 0;
  int pow2[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,
                32768,65536,131072,262144,524288,1048576,2097152,4194304,
                8388608,16777216,33554432};
  makeEmptyClause(c);
  while (cstr[idx] != '\0') {
    if (cstr[idx] == '~') {
      /* negative literal */
      idx++;
      n = expectLetter(cstr[idx++]);
      c->negative |= pow2[n];
    } else {
      /* positive literal */
      n = expectLetter(cstr[idx++]);
      c->positive |= pow2[n];
    }
    if (cstr[idx] == ',') {
      idx++;
      if (cstr[idx] == '\0') {
        fprintf(stderr, "Syntax error: truncated clause\n");
    exit(EXIT_FAILURE);
      }
    }
  }
}

int areEqualClauses(clause a, clause b) {
  /* returns TRUE if and only if the clauses a and b are the same */
  if ((a.positive == b.positive) && (a.negative == b.negative)) {
    return TRUE;
  }
  return FALSE;
}

void printClause(clause c) {
  /* prints clause c on standard output */
  int i, mask, trueflag, comma=0;
  printf("[");
  trueflag = 0;
  for (i=0,mask=1; i < 26; i++,mask*=2) {
    int cnt = 0;
    if (c.positive & mask) {
      if (comma) putchar(',');
      putchar('a'+i);
      comma = 1;
      cnt++;
    }
    if (c.negative & mask) {
      if (comma) putchar(',');
      printf("~%c", 'a'+i);
      comma = 1;
      cnt++;
    }
    trueflag |= (cnt == 2);
  }
  printf("]");
  if (trueflag) {
    printf("=TRUE");
  } else {
    if (isEmptyClause(c)) {
      printf("=FALSE");
    }
  }
}

void printlnClause(clause c) {
  /* prints clause c followed by a newline on standard output */
  printClause(c);
  putchar('\n');
}

/******** ADT for clauseSets ************************************/

void freeClauseSet(clauseSet set) {
  /* releases the memory allocated for set */
  free(set.clauses);
}

void makeEmptyClauseSet(clauseSet *set) {
  /* makes an empty clause set */
  set->size = 0;
  set->allocated = 0;
  set->clauses = NULL;
}

int isEmptyClauseSet(clauseSet s) {
  /* returns TRUE if and only if s is and empty set of clauses */
  return (s.size == 0 ? TRUE : FALSE);
}

int findIndexOfClause(clause c, clauseSet s) {
  /* returns index of clause c in set s, or -1 if c is not found */
  int i = 0;
  while (i < s.size && !areEqualClauses(s.clauses[i], c)) {
    i++;
  }
  return (i < s.size ? i : -1);
}

int isElementOfClauseSet(clause c, clauseSet s) {
  /* returns TRUE if and only if c is in set s */
  return (findIndexOfClause(c, s) == -1  ? FALSE : TRUE);
}

int containsEmptyClause(clauseSet s) {
  /* returns TRUE if and only if the set s contains the empty clause */
  clause empty;
  makeEmptyClause(&empty);
  return isElementOfClauseSet(empty, s);
}

int isClauseSubset(clauseSet a, clauseSet b) {
  /* returns TRUE if and only if a is a subset of b */
  int i;
  if (b.size < a.size) {
    return FALSE;
  }
  for (i=0; i < a.size; i++) {
    if (!isElementOfClauseSet(a.clauses[i], b)) {
      return FALSE;
    }
  }
  return TRUE;
}

void insertInClauseSet(clause c, clauseSet *s) {
  /* inserts clause s in set s */
  if (isElementOfClauseSet(c, *s)) {
    return;   /* clause was already in set */
  }
  if (s->size == s->allocated) {
    /* reallocation needed. */
    s->allocated += 128;
    s->clauses = realloc(s->clauses, s->allocated*sizeof(clause));
  }
  s->clauses[s->size++] = c;
}

void unionOfClauseSets(clauseSet *a, clauseSet b) {
  /* implements: a = union(a,b) */
  int i;
  for (i=0; i < b.size; i++) {
    insertInClauseSet(b.clauses[i], a);
  }
}

void crossClauses(clause a, clause b, clauseSet *rsv) {
  /* returns in rsv the set of clauses that are produced by 
   * resolving the positive literals of a with the negative literals
   * of b. Note that rsv must be an empty set, before 
   * calling this function.
   */
  int crossing = a.positive & b.negative;
  int mask = 1;
  while (crossing != 0) {
    if (crossing & mask) {
      clause c;
      c.positive = (a.positive | b.positive) & (~mask);
      c.negative = (a.negative | b.negative) & (~mask);
      insertInClauseSet(c, rsv);
    }
    crossing &= ~mask;
    mask *= 2;
  }
}

void printClauseSet(clauseSet s) {
  /* prints set of clauses s on standard output */  
  int i;
  printf("{");
  if (s.size > 0) {
    printClause(s.clauses[0]);
    for (i=1; i < s.size; i++) {
      printf(", ");
      printClause(s.clauses[i]);
    }
  }
  printf("}");
}

void printlnClauseSet(clauseSet s) {
  /* prints set of clauses s followed by a newline on standard output */  
  printClauseSet(s);
  putchar('\n');
}

/******** Main program ***************************************/

void resolveClauses(clause a, clause b, clauseSet *rsv) {
  /* returns the resolvents of the clauses a and b in the set rsv */
  makeEmptyClauseSet(rsv);
  crossClauses(a, b, rsv);
  crossClauses(b, a, rsv);
}

void resolution(clauseSet *kb) {
  /* Extends the kb with rules that can be inferred by resolution.
   * The function returns, as soon as it inferred the empty 
   * clause (i.e. false). The function also returns, if all possible
   * resolvents have been computed.
   */
  while (!containsEmptyClause(*kb)) {
    int i, j;
    clauseSet inferred;
    makeEmptyClauseSet(&inferred);
    for (i=0; i < kb->size; i++) {
      for (j=i+1; j < kb->size; j++) {
        clauseSet resolvents;
        resolveClauses(kb->clauses[i], kb->clauses[j], &resolvents);
        unionOfClauseSets(&inferred, resolvents);
        freeClauseSet(resolvents);
      }
    }
    if (isClauseSubset(inferred, *kb)) {
      break; /* No new clauses found */
    }
    unionOfClauseSets(kb, inferred);
    freeClauseSet(inferred);
  }
}

void init(clauseSet *s) {
  /* makes KB with clauses: {~a,~b}, {a,~b,~c,~d}, {b,~d}, {c,~d} */
  clause c;
  makeEmptyClauseSet(s);
  /* clause {~a,~b} */
  makeClause(&c, "~a,~b");
  insertInClauseSet(c, s);
  /* clause , {a,~b,~c,~d} */
  makeClause(&c, "a,~b,~c,~d");
  insertInClauseSet(c, s);
  /* clause {b,~d} */
  makeClause(&c, "b,~d");
  insertInClauseSet(c, s);
  /* clause , {c,~d} */
  makeClause(&c, "c,~d");
  insertInClauseSet(c, s);
  /* add clause {d}: negation of goal ~d */
  makeClause(&c, "d");
  insertInClauseSet(c, s);
}

/******** Part 1 ***************************************/

int isResolvent(clause c, clauseSet* s) {
    for(int i = 0; i < s->size; ++i) {
        if(areEqualClauses(c, s->clauses[i])) {
            return TRUE;
        }
    }
    return FALSE;
}

void printResolution(clause rsv, clause a, clause b) {
    printClause(rsv);
    printf(" is inferred from ");
    printClause(a);
    printf(" and ");
    printClause(b);
    printf(".\n");
}

void recursivePrintProof(int idx, clauseSet s) {
    /* Find two different clauses that resolve the clause at idx.
     * Then, find whatever resolved those clauses. If there are none,
     * we hit one of the initial clauses, and stop. */
    for(int a = 0; a < s.size; ++a) {
        for(int b = a + 1; b < s.size; ++b) {
            if(a == idx || b == idx) continue;

            clauseSet rsv;
            resolveClauses(s.clauses[a], s.clauses[b], &rsv);
            if(isResolvent(s.clauses[idx], &rsv)) {
                recursivePrintProof(a, s);
                recursivePrintProof(b, s);
                printResolution(s.clauses[idx], s.clauses[a], s.clauses[b]);
                return;
            }
        }
    }
}

/******** Part 2 ***************************************/

/* 
 * Checks if the input is in the correct format:
 * 'KB=[. . .]'
 */
int isValidInput(char* input) {
    if(input[0] == 'K' && input[1] == 'B' 
        && input[2] == '=' && input[3] == '[') return TRUE;
    fprintf(stderr, "Syntax Error: Invalid input string. Must be in the form \"KB=[...]\".\n");
    return FALSE;
}

/*
 * Makes a clause from a given input. Returns forwarded index to
 * next clause.
 */
int makeClauseFromString(clause* c, char* input, int idx) {
    if(input[idx] == '[') ++idx;

    int i = 0;
    while(input[idx] != ']') {
        ++i; ++idx;
    }
    char* s = malloc((i+1) * sizeof(char));
    strncpy(s, input+idx-i, i);
    makeClause(c, s);
    free(s);
    idx+=2;
    return idx;
}

/*
 * Returns false when loading failed.
 */
int loadKB(clauseSet* kb, char* input) {
    makeEmptyClauseSet(kb);
    if(!isValidInput(input)){
        return FALSE;
    }

    int idx = 4;
    while(input[idx] != '\0') {
        if(input[idx] != '[') {
            fprintf(stderr, "Syntax Error: Clauses must be listed in the form \"[[...],[...],[...]]\".\n");
            return FALSE;
        }

        clause c;
        makeEmptyClause(&c);
        idx = makeClauseFromString(&c, input, idx);
        insertInClauseSet(c, kb);
    }
    return TRUE;
}

void printProof(clauseSet s) {
  int idx;
  clause empty;
  makeEmptyClause(&empty);
  idx = findIndexOfClause(empty, s);
  recursivePrintProof(idx, s);
}

/******** BONUS INTEGRATION ***************************************/

/* most of the code is in a sepparate file called 'CNFify.c'*/

/* Decodes the structure parsed by CNFify */

void loadKBfromRegex(char* regex) {

}

           
int main(int argc, char *argv[]) {
  clauseSet kb;
  if(argc >= 2) {
    if(!loadKB(&kb, argv[1])){
        return 1;
    }
  } else {
      init(&kb);
  }
  printf("KB=");
  printlnClauseSet(kb);
  resolution(&kb);
  printf("KB after resolution=");
  printlnClauseSet(kb);
  if (containsEmptyClause(kb)) {
    printf("Resolution proof completed.\n");
    printf("\nProof:\n");
    printProof(kb);
  } else {
    printf("Resolution proof failed.\n");
  }
  freeClauseSet(kb);
  return EXIT_SUCCESS;
}