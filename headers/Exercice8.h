#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/exercice4.h"
#include "../headers/Exercice3.h"
#include "../headers/main.h"
#include "../headers/Exercice2.h"
#include "../headers/Exercice6.h"
#include "../headers/Exercice5.h"

#ifndef EXERCICE8_H
#define EXERCICE8_H

void initBranch();
int branchExists(char* branch);
void createBranch(char* branch);
char* getCurrentBranch();
char* hashToPathCommit(char* hash);
void printBranch(char* branch);
List* branchList(char* branch);
List* getAllCommits();

#endif