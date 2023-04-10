#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/exercice4.h"
#include "../headers/Exercice3.h"
#include "../headers/main.h"
#include "../headers/Exercice2.h"

#ifndef EXERCICE5_H
#define EXERCICE5_H

#include "exercice4.h"

char* hashToFile(char* hash);
char* blobWorkTree(WorkTree* wt);
char* concat_paths(char* path1, char* path2);
char* saveWorkTree(WorkTree* wt, char* path);
int isWorkTree(char* hash);
void restoreWorkTree(WorkTree* wt, char* path);

#endif
