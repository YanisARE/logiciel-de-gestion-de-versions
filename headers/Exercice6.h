#include "../headers/Exercice5.h"

#ifndef EXERCICE6_H
#define EXERCICE6_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct key_value_pair kvp;
typedef struct hash_table HashTable;
typedef HashTable Commit;

kvp* createKeyVal(char* key, char* val);
void freeKeyVal(kvp* kv);
char* kvts(kvp* k);
kvp* stkv(char* str);
Commit* initCommit();
unsigned long djb2(unsigned char *str);
void commitSet(Commit* c, char* key, char* value);
Commit* createCommit(char* hash, int Ncommit);
char* commitGet(Commit* c, char* key);
char* cts(Commit* c);
Commit* stc(char* ch);
Commit* ftc(char* file);
void ctf(Commit* c, char* file);
char* blobCommit(Commit* c);

#endif 
