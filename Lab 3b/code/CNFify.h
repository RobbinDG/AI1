typedef enum {
    AND, OR, IMPLICATION, BIIMPLICATION, T, F, NONE
} logicOperator;

typedef struct literal {
    char predicate;
    int negated;
} literal;

typedef struct sentence sentence;

typedef struct sentence {
    sentence* left;
    sentence* right;
    char predicate;
    logicOperator operator;
    int isSingular;
    int negated;
} sentence;

void throwSyntaxError() {
    fprintf(stderr, "Syntax Error: while parsing sentence. Exiting...\n");
    exit(1);
}

sentence* convertToCNF(char* regex);