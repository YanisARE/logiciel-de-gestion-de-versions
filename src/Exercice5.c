#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/exercice5.h"

//Exercice 5
char* hashToFile(char* hash){
    struct stat st;
    char* ch2 = strdup(hash); 
    ch2[2] = '\0';
    if (stat(ch2, &st) == -1) {
        mkdir(ch2);
    }
    return hashToPath(hash);
}

char* blobWorkTree(WorkTree* wt){
    char fname [100] = "/tmp/myfileXXXXXX"; 
    int fd = mkstemp(fname);
    wttf(wt, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".t");
    cp(ch, fname);
    return hash;
}


//Q2
char* concat_paths(char* path1, char* path2) {
    char * res = malloc(strlen(path1) + strlen(path2) + 1); 
    if (res == NULL) {
        printf("Erreur lors de l'allocation\n");
        return NULL; 
    }
    strcpy(res, path1);
    strcat(res, "/");
    strcat(res, path2);
    return res;
}

#include <sys/stat.h>

char* saveWorkTree(WorkTree* wt, char* path) {
    struct stat path_stat;
    for (int i = 0; i < wt->n; i++) {
        char* absPath = concat_paths(path, wt->tab[i].name);
        if (stat(absPath, &path_stat) == 0) { // vérifie si la fonction stat récupère bien les informations du fichier
            if (S_ISREG(path_stat.st_mode)) { // Vérifie si c'est un fichier
                blobFile(absPath);
                wt->tab[i].hash = sha256file(absPath);
                wt->tab[i].mode = getChmod(absPath);
            }
        } else {
            WorkTree* wt2 = initWorkTree();
            List* L = listdir(absPath);
            for (Cell* ptr = *L; ptr != NULL; ptr = ptr->next) {
                if (ptr->data[0] == '.')
                    continue;
                struct stat dir_stat;
                if (stat(absPath, &dir_stat) == 0) {
                    mode_t dir_mode = dir_stat.st_mode & (0x1C0 | 0x38 | 0x07);
                    appendWorkTree(wt2, ptr->data, 0, dir_mode);
                }
            }
            wt->tab[i].hash = saveWorkTree(wt2, absPath);
            wt->tab[i].mode = getChmod(absPath);
        }
    }
    return blobWorkTree(wt);
}


//Q3
int isWorkTree(char* hash){
    if (file_exists(strcat(hashToPath(hash), ".t"))){
        return 1; 
    }
    if (file_exists(hashToPath(hash))){ 
        return 0;
    }
    return -1;
}

void restoreWorkTree(WorkTree* wt, char* path){
    for(int i=0; i < wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name); 
        char* copyPath = hashToPath(wt->tab[i].hash);
        char* hash = wt->tab[i].hash;
        if (isWorkTree(hash) == 0){
            cp(absPath , copyPath);
            setMode(getChmod(copyPath),absPath); 
        }else{
            if(isWorkTree(hash) == 1){
                strcat(copyPath, ".t");
                WorkTree* nwt = ftwt(copyPath); 
                restoreWorkTree(nwt, absPath); 
                setMode(getChmod(copyPath),absPath);
            }
        }
    }
}


