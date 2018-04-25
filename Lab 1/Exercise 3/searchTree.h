#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

typedef struct TreeNode *Tree;
struct TreeNode {
int item;
Tree leftChild, rightChild;
};

Tree emptyTree();
Tree newTree(int n, Tree tL, Tree tR);
Tree searchNode(Tree t, int n);
Tree addInSearchTree(Tree t, int n);
int isInSearchTree (Tree t, int n);
void printSearchTree (Tree t);
void checkSearchTree (Tree t, int size);
void freeSearchTree (Tree t);

#endif
