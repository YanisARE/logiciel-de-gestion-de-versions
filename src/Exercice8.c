#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/Exercice8.h"


//Exercice 8
/*8.1-cr´ee le fichier cach´e .current branch contenant
la chaˆıne de caract`eres ”master
*/
void initBranch(){
    FILE* f = fopen("branche/.currentbranch", "w"); 
    fputs("master",f);
    fclose(f);
}

/*8.2-v´erifie l’existence d’une branche
*/
int branchExists(char* branch){
    if(branch == NULL){
        return 0;
    }
    List* refs = listdir(".refs");
    return searchList(refs, branch) != NULL;
}

/*8.3-cr´ee une r´ef´erence appel´ee branch,
qui pointe vers le mˆeme commit que la r´ef´erence HEAD
*/
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
/*8.5- lit le fichier cach´e .current branch pour
retourner le nom de la branche courante
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

/*Q.Annexe: Transforme un hash en chemin
*/
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
/*8.6-parcourt la branche appel´ee
branch, et pour chacun de ses commits, affiche son hash et son message descriptif 
*/
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
    char * path = malloc(6 + strlen(branch));
    strcat(path, ".refs/");
    strcat(path, branch);
    Commit* c = ftc(path); 
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
    free(path);
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
/*8.6-contruit et retourne une liste
chaˆın´ee contenant le hash de tous les commits de la branche
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

/*8.7-renvoie la liste des hash des commits de
toutes les branches (sans r´ep´etition).
*/
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

//Fonction Annexe : Supprime le fichier dont le nom a ete passe en parametre 
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