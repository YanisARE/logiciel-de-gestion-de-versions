#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifndef EXERCICE2_H
#define EXERCICE2_H

typedef struct cell {
    char * data ;
    struct cell * next ;
} Cell ;
typedef Cell * List ;

List* listdir(char* root_dir) ;
int file_exists(char *file) ;
void cp(char *to, char *from) ;
char* hashToPath(char* hash) ;
void blobFile(char* file) ;

#endif
