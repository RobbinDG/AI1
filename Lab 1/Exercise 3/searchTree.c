#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "searchTree.h"

Tree emptyTree() {
return NULL;
}

Tree newTree(int n, Tree tL, Tree tR) {
Tree new = malloc(sizeof(struct TreeNode));
assert(new != NULL);
new->item = n;
new->leftChild = tL;
new->rightChild = tR;
return new;
}

Tree searchNode(Tree t, int n) {
if (t == NULL) {
return NULL;
}
if (n == t->item) {
return t;
}
if ( n < t->item ) {
t = t->leftChild;
} else {
t = t->rightChild;
}
return searchNode(t,n);
}

Tree addInSearchTree(Tree t, int n) {
if (t == NULL) {
return newTree(n,emptyTree(), emptyTree());
}
if (n < t->item){
t->leftChild = addInSearchTree(t->leftChild,n);
} else if (n > t->item) {
t->rightChild = addInSearchTree(t->rightChild,n);
}
return t;
}

int isInSearchTree (Tree t, int n)
{
	if (t == NULL)
		return 0;
	if (t->item == n)
		return 1;
	else if (t->item < n)
		return isInSearchTree (t->rightChild, n);
	else
		return isInSearchTree(t->leftChild, n);
}

void printSearchTree (Tree t)
{
	if (t == NULL)
		return;
	printSearchTree(t->leftChild);
	printf("%d ", t->item);
	printSearchTree(t->rightChild);
}

void checkSearchTree (Tree t, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (searchNode(t, i) == NULL)
		{
			printf("%d ", i);
		}
	}
}

void freeSearchTree (Tree t)
{
	if (t == NULL)
		return;
	freeSearchTree (t->leftChild);
	freeSearchTree (t->rightChild);
	free(t);
}

