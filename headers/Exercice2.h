#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/main.h"
#ifndef EXERCICE2_H
#define EXERCICE2_H

typedef struct cell {
    char * data ;
    struct cell * next ;
} Cell ;
typedef Cell * List ;

List * initList();
List buildCell(char * ch);
void insertFirst(List* L, List C);
char* ctos(List c);
char* ltos(List* L);
Cell* listGet(List* L, int i);
Cell* searchList(List* L, char * str);
Cell** stol(char *s);
Cell** ftol(char *path);

#endif
