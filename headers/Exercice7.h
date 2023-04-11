#include "../headers/Exercice6.h"
#ifndef EXERCICE7_H
#define EXERCICE7_H


void initRefs();
void createUpdateRef(char* ref , char* hash);
void deleteRef ( char * ref );
char * getRef ( char * ref_name );
void createFile ( char * file );
void myGitAdd ( char * File_or_Folder );
void myGitCommit ( char * branch_name , char * message );

#endif