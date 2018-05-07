#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "searchTree.h"

Tree emptyTree() {
return NULL;
}

Tree newTree(State state, Tree tL, Tree tR) {
Tree new = malloc(sizeof(struct TreeNode));
assert(new != NULL);
new->state.value = state.value;
new->state.cost = state.cost;
new->state.action = state.action;
new->state.pathParent = state.pathParent;
new->leftChild = tL;
new->rightChild = tR;
return new;
}

Tree addInSearchTree(Tree t, State state) {
if (t == NULL) {
return newTree(state, emptyTree(), emptyTree());
}
if (state.value < t->state.value){
t->leftChild = addInSearchTree(t->leftChild,state);
} else if (state.value > t->state.value) {
t->rightChild = addInSearchTree(t->rightChild,state);
}
return t;
}

Tree addNodeInSearchTree (Tree t, Tree newT) {
if (t == NULL) {
return newT;
}
if (newT->state.value < t->state.value){
t->leftChild = addNodeInSearchTree(t->leftChild,newT);
} else if (newT->state.value > t->state.value) {
t->rightChild = addNodeInSearchTree(t->rightChild,newT);
}
return t;
}

int isInSearchTree (Tree t, State state)
{
	if (t == NULL)
		return 0;
	if (t->state.value == state.value)
		return 1;
	else if (t->state.value < state.value)
		return isInSearchTree (t->rightChild, state);
	else
		return isInSearchTree(t->leftChild, state);
}

int isInSearchTreeN (Tree t, int n)
{
	if (t == NULL)
		return 0;
	if (t->state.value == n)
		return 1;
	else if (t->state.value < n)
		return isInSearchTreeN (t->rightChild, n);
	else
		return isInSearchTreeN (t->leftChild, n);
}

Tree searchNode(Tree t, State state) {
if (t == NULL) {
return NULL;
}
if (state.value == t->state.value) {
return t;
}
if ( state.value < t->state.value ) {
t = t->leftChild;
} else {
t = t->rightChild;
}
return searchNode(t,state);
}

void printSearchTree (Tree t)
{
	if (t == NULL)
		return;
	printSearchTree(t->leftChild);
	printf("%d ", t->state.value);
	printSearchTree(t->rightChild);
}

void freeSearchTree (Tree t)
{
	if (t == NULL)
		return;
	freeSearchTree (t->leftChild);
	freeSearchTree (t->rightChild);
	free(t);
}

