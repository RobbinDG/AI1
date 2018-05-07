#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H
#include "state.h"


Tree emptyTree();
Tree newTree(State s, Tree tL, Tree tR);
Tree addInSearchTree(Tree t, State s);
Tree addNodeInSearchTree (Tree t, Tree newT);
int isInSearchTree (Tree t, State state);
int isInSearchTreeN (Tree t, int n);
Tree searchNode(Tree t, State state);
void freeSearchTree (Tree t);
void printSearchTree (Tree t);

#endif
