#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/Exercice5.h"

//Exercice 5
//Q1
char* hashToFile(char* hash){
    struct stat st;
    char* ch2 = strdup(hash); 
    ch2[2] = '\0';
    if (stat(ch2, &st) == -1) {
        mkdir(ch2,0700);
    }
    return hashToPath(hash);
}
/*
Q1-crée un fichier temporaire
représentant le WorkTree pour pouvoir ensuite créer l’enregistrement instantané du WorkTree (avec
l’extension ”.t”
*/
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


//Concatene les deux chemin passe en parametre
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

/*Q2-etant donné un WorkTree dont le chemin est donné en paramètre, 
crée un enregistrement instantané de tout son contenu (de manière récursive), puis de lui même
L’appel à saveWorkTree permet de conserver une sauvegarde de l’état de plusieurs fichiers à un instant
donné
*/
char* saveWorkTree(WorkTree* wt, char* path) {
    struct stat path_stat;
    for (int i = 0; i < wt->n; i++) {
        printf("Avant char* absPath = concat_paths(path, wt->tab[i].name);\n");
        char* absPath = concat_paths(path, wt->tab[i].name);
        printf("Apres char* absPath = concat_paths(path, wt->tab[i].name);\n");
        if (stat(absPath, &path_stat) == 0) { // vérifie si la fonction stat récupère bien les informations du fichier
            printf("Apres stat(absPath, &path_stat) == 0\n");
            if (S_ISREG(path_stat.st_mode)) { // Vérifie si c'est un fichier
                printf("Apres S_ISREG(path_stat.st_mode et avant blobfile)\n");
                blobFile(absPath);
                printf("blobfile\n");
                wt->tab[i].hash = sha256file(absPath);
                printf("apres sha256file\n");
                wt->tab[i].mode = getChmod(absPath);
                printf("getChmod\n");
            }
        } else {
            WorkTree* wt2 = initWorkTree();
            printf("apres initWorktree\n");
            List* L = listdir(absPath);
            if(L==NULL) return NULL;
            printf("L dans saveworktree vaut : %p",L);
            printf(" apres listdir\n");
            printf("ptr -> next =%p\t",(*L)->next);
            printf(" apres printf de ptr->next\n");
            for (List ptr = *(L); ptr != NULL; ptr = ptr->next) {
                printf(" apres for loop\n");
                if (ptr->data[0] == '.')
                    continue;
                struct stat dir_stat;
                if (stat(absPath, &dir_stat) == 0) {
                    printf("apres if stat(absPath, &dir_stat) == 0\n");
                    mode_t dir_mode = dir_stat.st_mode & (0x1C0 | 0x38 | 0x07);
                    appendWorkTree(wt2, ptr->data, 0, dir_mode);
                    printf("apres appendWorktree\n");
                }
            }
            printf("avant saveWorkTree\n");
            wt->tab[i].hash = saveWorkTree(wt2, absPath);
            printf("apres getChmod\n");
            wt->tab[i].mode = getChmod(absPath);
        }
    }
    printf("avant blobWorkTree(wt)\n");
    return blobWorkTree(wt);
}


//QAnnexe- Permet de verifier l'existence du worktree
int isWorkTree(char* hash){
    if (file_exists(strcat(hashToPath(hash), ".t"))){
        return 1; 
    }
    if (file_exists(hashToPath(hash))){ 
        return 0;
    }
    return -1;
}
/*Q3-restaurer un WorkTree c’est-à-dire qui recrée l’arborescence des
fichiers comme décrit par ses enregistrements instantanés
*/
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


