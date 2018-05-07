#ifndef STATE_H
#define STATE_H

/* The type State is a data type that represents a possible state 
 * of a search problem. It can be a complicated structure, but it 
 * can also be a simple type (like int, char, ..). 
 * Note: if State is a structure, make sure that the structure does not 
 *       contain pointers!
 */
 
#define PLUS1 1
#define MIN1 2
#define TIMES2 3
#define TIMES3 4 
#define DIV2 5
#define DIV3 6

typedef struct TreeNode *Tree;

typedef struct {
  	int value;
	int cost;
	int action;
	int depth;
	Tree pathParent;
} State;

struct TreeNode {
	State state;
	Tree leftChild, rightChild;
};



  


#endif

