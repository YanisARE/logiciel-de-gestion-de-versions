#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#ifndef MAIN_H
#define MAIN_H

int hashFile(char* source, char* dest);
char* sha256file(char* file);  

#endif

#ifndef EX3_MAIN
int main();
#endif