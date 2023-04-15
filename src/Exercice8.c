#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/Exercice8.h"


//Exercice 8
//1
void initBranch(){
    FILE* f = fopen("branche/.currentbranch", "w"); 
    fputs("master",f);
    fclose(f);
}

//2
int branchExists(char* branch){
    List* refs = listdir(".refs");
    return searchList(refs, branch) != NULL;
}

//3
void createBranch(char* branch){
    char* hash = getRef("HEAD");
    createUpdateRef(branch , hash);
}

//4
/* Version segmenté
char* getCurrentBranch(){
    FILE* f = fopen(".current_branch", "r"); 
    char* buff = malloc(sizeof(char)*100);
    fscanf(f,"%s",buff);
    return buff;
}
*/
char* getCurrentBranch() {
    FILE* f = fopen(".current_branch", "r");
    if (f == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier .current_branch\n");
        return NULL;
    }
    char* buff = malloc(sizeof(char)*100);
    if (buff == NULL) {
        printf("Erreur: impossible d'allouer de la mémoire pour la chaine de caractères\n");
        return NULL;
    }
    if (fscanf(f, "%s", buff) != 1) {
        printf("Erreur: impossible de lire la branche courante\n");
        free(buff);
        return NULL;
    }
    return buff;
}

//5
char* hashToPathCommit(char* hash){
    if (hash == NULL) {
        return NULL;
    }
    char* buff = malloc(sizeof(char)*100);
    sprintf(buff, "%s.c", hashToPath(hash));
    return buff;
}
/* Version segmenté
void printBranch(char* branch){
    char* commit_hash = getRef(branch);
    Commit* c = ftc(hashToPathCommit(commit_hash)); 
    while (c != NULL){
        if (commitGet(c, "message")!=NULL){
            printf("%s -> %s \n" ,commit_hash , commitGet(c, "message"));
        }else{
            printf("%s \n", commit_hash);
        }
        if (commitGet(c, "predecessor")!=NULL){
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash));
        }else{
            c = NULL;
        }
    }
}
*/
//Fonction non segmenté avec des conditions pour eviter les segmentations fault
void printBranch(char* branch) {
    if (branch == NULL) {
        printf("Erreur: nom de branche invalide\n");
        return;
    }
    
    char* commit_hash = getRef(branch);
    if (commit_hash == NULL) {
        printf("Erreur: la branche n'existe pas\n");
        return;
    }
    
    Commit* c = ftc(hashToPathCommit(commit_hash)); 
    while (c != NULL){
        if (commitGet(c, "message")!=NULL){
            printf("%s -> %s \n" ,commit_hash , commitGet(c, "message"));
        }else{
            printf("%s \n", commit_hash);
        }
        if (commitGet(c, "predecessor")!=NULL){
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash));
        }else{
            c = NULL;
        }
    }
}

//6
/* Version segmenté
List* branchList(char* branch){
    List* L = initList();
    char* commit_hash = getRef(branch);
    Commit* c = ftc(hashToPathCommit(commit_hash)); 
    while (c != NULL){
        insertFirst(L, buildCell(commit_hash)); 
        if (commitGet(c, "predecessor")!=NULL){
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash)); 
        }else{
            c = NULL;
        }
    }
    return L;
}
*/
List* branchList(char* branch){
    List* L = initList();
    char* commit_hash = getRef(branch);
    Commit* c = ftc(hashToPathCommit(commit_hash)); 
    while (c != NULL){
        insertFirst(L, buildCell(commit_hash)); 
        if (c != NULL && commitGet(c, "predecessor")!=NULL){
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash)); 
        }else{
            c = NULL;
        }
    }
    return L;
}

//7
List* getAllCommits(){
    List* L = initList();
    List* content = listdir(".refs");
    for(Cell* ptr = *content; ptr != NULL; ptr = ptr->next){
        if (ptr->data[0] == '.'){continue;}
        List* list = branchList(ptr->data);
        Cell* cell = *list;
        while (cell != NULL){
            if (searchList(L, cell->data) == NULL){ 
                insertFirst(L,buildCell(cell->data));
            }
            cell = cell->next; 
        }
    }
    return L;
}

//Fonction Annexe : 

void delete(char *filename) {
    if (filename == NULL) {
        printf("Le nom de fichier est vide\n");
        return;
    }
    
    if (remove(filename) == 0) {
        printf("Le fichier '%s' a été supprimé avec succès\n", filename);
    } else {
        perror("Erreur lors de la suppression du fichier");
    }
}