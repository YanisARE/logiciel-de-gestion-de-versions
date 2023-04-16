#include "../headers/Exercice4.h"
#include "../headers/Exercice5.h"

typedef struct Node {
    char* value;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} List;

void addToList(List** list, char* value);
int isInConflict(char* name, WorkTree* wt1, WorkTree* wt2);
WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts)
