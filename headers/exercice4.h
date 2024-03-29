#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "../headers/Exercice3.h"

#ifndef EXERCICE4_H
#define EXERCICE4_H

typedef struct {
    char * name ;
    char * hash ;
    int mode ;
} WorkFile ;

typedef struct {
    WorkFile * tab ;
    int size ;
    int n ;
} WorkTree ;

int getChmod(const char* path);
void setMode(int mode, char* path);
WorkFile* createWorkFile(char* name);
char* wfts(WorkFile* wf);
WorkFile* stwf(char* ch);
WorkTree* initWorkTree();
int inWorkTree(WorkTree* wt, char* nom);
int appendWorkTree(WorkTree* wt, char* nom, char* hachage, int mode);
char* wtts(WorkTree* wt);
WorkTree* stwt(char* ch);
int wttf(WorkTree* wt, char* fichier);
WorkTree* ftwt(char* fichier);

#endif

