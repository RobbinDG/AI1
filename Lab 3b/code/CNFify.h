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

void printSentence(sentence* s);
sentence* convertToCNF(char* regex);
void freeSentence(sentence* s);