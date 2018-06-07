#include <stdlib.h>
#include <stdio.h>

#include "CNFify.h"

#define TRUE 1
#define FALSE 0

/*
 * We are using the following grammar:
 *  <sentence> ::= [ '~' ] [ '[' ] <literal>|<sentence> [ <operator> <literal>|<sentence> ] [ ']' ]
 *  <literal>  ::= [ '~' ] <letter>
 *  <letter>   ::= 'a'|'b'|...|'z'
 */

void throwSyntaxError() {
    fprintf(stderr, "Syntax Error: while parsing sentence. Exiting...\n");
    exit(1);
}

sentence* newSentence() {
    sentence* s = malloc(sizeof(sentence));
    s->left = NULL;
    s->right = NULL;
    s->predicate = '-';
    s->operator = NONE;
    s->isSingular = FALSE;
    s->negated = FALSE;
    return s;
}

void newLiteral(literal* l) {
    l->predicate = '-';
    l->negated = FALSE;
}

int acceptCharacter(char* input, int* p, char c) {
    if(input[*p] == c) {
        ++(*p);
        return TRUE;
    }
    return FALSE;
}

int acceptLiteral(char* input, int* p, sentence* s) {
    if(input[*p] >= 'a' && input[*p] <= 'z') {
        s->predicate = input[*p];
        s->isSingular = TRUE;
        ++(*p);
        return TRUE;
    }
    return FALSE;
}

int acceptOperator(char* input, int* p, logicOperator* n) {
    if(acceptCharacter(input, p, '*')) {
        *n = AND;
        return TRUE;
    } else if(acceptCharacter(input, p, '+')) {
        *n = OR;
        return TRUE;
    } else if(acceptCharacter(input, p, '=') &&
              acceptCharacter(input, p, '-')) {
        *n = IMPLICATION;
        return TRUE;
    } else if(acceptCharacter(input, p, '-') &&
              acceptCharacter(input, p, '=') &&
              acceptCharacter(input, p, '-')) {
        *n = BIIMPLICATION;
        return TRUE;
    }
    printf("here\n");
    return FALSE;
}

int acceptSentence(char* input, int* p, sentence** parent) {
    sentence* s = newSentence();
    int bracketsFlag = FALSE;

    if(acceptCharacter(input, p, '~')) s->negated = TRUE;
    if(acceptLiteral(input, p, s)) {
        s->isSingular = TRUE;
        *parent = s;
        return TRUE;
    } 
    if(acceptCharacter(input, p, '[')) bracketsFlag = TRUE;
    if(!acceptSentence(input, p, &(s->left))) throwSyntaxError();
    logicOperator n = NONE;
    if(acceptOperator(input, p, &n)) {
        if(!acceptSentence(input, p, &(s->right))) throwSyntaxError();
    } else {
        s->isSingular = TRUE;
    }
    s->operator = n;
    if((s->operator == OR && input[*p] == '+') || (s->operator == AND && input[*p] == '*')) {
        sentence* t = newSentence();
        ++(*p);
        t->left = s->left;
        t->right = s->right;
        t->operator = s->operator;
        s->left = t;
        if(!acceptSentence(input, p, &(s->right))) throwSyntaxError();
    }
    if(bracketsFlag && !acceptCharacter(input, p, ']')) throwSyntaxError();
    // append created sentence to parent
    *parent = s;
    return TRUE;
}

/* Parses a sentence from a given input string. */
sentence* parseSentence(char* input) {
    sentence* s;
    int* p = malloc(sizeof(int));
    *p = 0;
    acceptSentence(input, p, &s);
    free(p);
    return s;
}

void printSentence(sentence* s) {
    if(s->negated) {
        printf("!");
    }
    printf("(");
    if(s->isSingular) {
        if(s->predicate != '-') {
            printf("%c", s->predicate);
        } else {
            printSentence(s->left);
        }
    } else {
        printSentence(s->left);
        printf("%d", s->operator);
        printSentence(s->right);
    }
    printf(")");
}

sentence* copy(sentence* c) {
    sentence* s = newSentence();
    s->operator = c->operator;
    s->isSingular = c->isSingular;
    s->negated = c->negated;
    if(c->isSingular) {
        s->predicate = c->predicate;
    } else {
        s->left = copy(c->left);
        s->right = copy(c->right);
    }
    return s;
}

int isAtomic(sentence *s) {
    if(s->isSingular) return TRUE;
    if(s->negated) return FALSE;
    return (isAtomic(s->left) && isAtomic(s->right));
}

void distribute(sentence *s) {
    // distributes over OR, assumes only negated atoms
    s->operator = AND;
    sentence* l = newSentence();
    sentence* r = newSentence();
    l->operator = OR;
    r->operator = OR;
    if(s->left->operator == AND) {
        l->left = s->left->left;
        l->right = copy(s->right);
        r->left = s->left->right;
        r->right = s->right;
    } else {
        l->left = s->right->left;
        l->right = copy(s->left);
        r->left = s->right->right;
        r->right = s->left;
    }
    s->left = l;
    s->right = r;
}

void correctToCNF(sentence *s) {
    if(s->isSingular) return;
    if(s->operator == OR) {
        if(s->left->operator == AND || s->right->operator == AND) {
            distribute(s);
        }
    }
    correctToCNF(s->left);
    correctToCNF(s->right);
}

int isCNF(sentence *s, logicOperator p) {
    // intialize with p == AND
    if(s->isSingular) return TRUE;
    if(s->operator == AND && p == OR) return FALSE;
    return (isCNF(s->left, s->operator) && isCNF(s->right, s->operator));
}

void flipAndOr(sentence *s) {
    if(s->operator == OR)
        s->operator = AND;
    else
        s->operator = OR;
}

void pushNegations(sentence* s) {
    if(s->isSingular) return;
    if(s->negated) {
        s->negated = FALSE;
        s->left->negated = !s->left->negated;
        s->right->negated = !s->right->negated;
        flipAndOr(s);
    }
    pushNegations(s->left);
    pushNegations(s->right);
}

void deMorgan(sentence *s) {
    while(!isAtomic(s)) {
        pushNegations(s);
    }
}

void eliminateImplication(sentence *s) {
    if(s->isSingular) return;
    if(s->operator == IMPLICATION) {
        s->operator = OR;
        s->left->negated = !s->left->negated;
    }
    eliminateImplication(s->left);
    eliminateImplication(s->right);
}

void eliminateBiimplication(sentence* s) {
    if(s->isSingular) return;
    if(s->operator == BIIMPLICATION) {
        sentence* l = newSentence();
        sentence* r = newSentence();
        s->operator = AND;
        l->operator = r->operator = IMPLICATION;
        l->left = s->left;
        l->right = s->right;
        r->left = copy(s->right);
        r->right = copy(s->left);
        s->left = l;
        s->right = r;
    }
    eliminateBiimplication(s->left);
    eliminateBiimplication(s->right);
}

void freeSentence(sentence* s) {
    if(s->isSingular) {
        free(s);
        return;
    }
    freeSentence(s->left);
    freeSentence(s->right);
    free(s);
}

sentence* convertToCNF(char* regex) {
    sentence *s = parseSentence(regex);
    eliminateBiimplication(s);
    eliminateImplication(s);
    deMorgan(s);
    while(!isCNF(s, AND)) {
        correctToCNF(s);
    }
    return s;
}

// int main(int argc, char *argv[]) {
//     sentence *s = parseSentence("(!(a*b)<=>!c)");
//     printSentence(s);
//     putchar('\n');
//     printf("%d\n", isCNF(s, AND));
//     convertToCNF(s);
//     printSentence(s);
//     putchar('\n');
//     return 0;
// }
