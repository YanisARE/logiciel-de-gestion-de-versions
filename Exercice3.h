#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define EXERCICE3_H
#define EX3

#ifndef EXERCICE3_H

typedef struct List {
    char **names;
    int size;
} List;

List* listdir(char* root_dir) ;
char* hashToPath(char* hash) ;
int file_exists(char *file) ;
void cp(char *to, char *from) ;
char* hashToPath(char* hash) ;
void blobFile(char* file) ;

#endif
