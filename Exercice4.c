#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
 
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

int getChmod(const char* path) {
    struct stat ret;
    
    if (stat(path, &ret) == -1) {
        return -1;
    }
    
    return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | /* owner */
           (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) | /* group */
           (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH); /* other */
}

void setMode(int mode, char* path) {
    char buff[100];
    sprintf(buff, "chmod %o %s", mode, path);
    system(buff);
}

WorkFile* createWorkFile(char* name){
    WorkFile * work= (WorkFile * ) malloc(sizeof(WorkFile));
    work->name=strdup(name);
    work->hash=NULL;
    work->mode=0;
    return work;
}