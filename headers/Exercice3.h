#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/Exercice2.h"


#ifndef EXERCICE3_H
#define EXERCICE3_H

void freeList(List* L);
List* listdir(char* root_dir) ;
char* hashToPath(char* hash) ;
int file_exists(char *file) ;
void cp(char *to, char *from) ;
char* hashToPath(char* hash) ;
void blobFile(char* file) ;

#endif
