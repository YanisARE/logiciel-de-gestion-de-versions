#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/Exercice8.h"


// Exercice 8
/* 8.1- crée le fichier caché .current branch contenant
la chaîne de caractères "master" */
void initBranch(){
    // On ouvre le fichier .currentbranch en mode écriture
    FILE* f = fopen("branche/.currentbranch", "w"); 
    // On écrit "master" dans le fichier
    fputs("master",f);
    // On ferme le fichier
    fclose(f);
}

/* 8.2- vérifie l’existence d’une branche */
int branchExists(char* branch){
    if(branch == NULL){
        return 0;
    }
    // On récupère la liste des références dans le dossier .refs
    List* refs = listdir(".refs");
    // On recherche la branche dans la liste des références
    return searchList(refs, branch) != NULL;
}

/* 8.3- crée une référence appelée branch,
qui pointe vers le même commit que la référence HEAD */
void createBranch(char* branch){
    // On récupère le hash du commit pointé par HEAD
    char* hash = getRef("HEAD");
    // On crée/met à jour la référence de la branche avec le hash récupéré
    createUpdateRef(branch , hash);
}

//4
/* Version segmenté
char* getCurrentBranch(){
    FILE* f = fopen(".current_branch", "r"); 
    char* buffer = malloc(sizeof(char)*100);
    fscanf(f,"%s",buffer);
    return buffer;
}

/* 8.5- lit le fichier caché .current branch pour
retourner le nom de la branche courante */
char* getCurrentBranch() {
    // ouverture du fichier .current_branch en mode lecture
    FILE* f = fopen(".current_branch", "r");
    if (f == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier .current_branch\n");
        return NULL;
    }
    // allocation mémoire pour stocker le nom de la branche courante
    char* buffer = malloc(sizeof(char)*100);
    if (buffer == NULL) {
        printf("Erreur: impossible d'allouer de la mémoire pour la chaine de caractères\n");
        return NULL;
    }
    // lecture de la branche courante depuis le fichier
    if (fscanf(f, "%s", buffer) != 1) {
        printf("Erreur: impossible de lire la branche courante\n");
        free(buffer);
        return NULL;
    }
    // On retourne le nom de la branche courante
    return buffer;
}

/* Q.Annexe: Transforme un hash en chemin */
char* hashToPathCommit(char* hash){
    if (hash == NULL) {
        return NULL;
    }
    // On alloue de la mémoire pour stocker le chemin du commit
    char* buffer = malloc(sizeof(char)*100);
    // On convertit le hash en chemin
    sprintf(buffer, "%s.c", hashToPath(hash));
    return buffer;
}

/* Version segmenté
void printBranch(char* branch){
    char* hash_commit = getRef(branch);
    Commit* c = ftc(hashToPathCommit(hash_commit)); 
    while (c != NULL){
        if (commitGet(c, "message")!=NULL){
            printf("%s -> %s \n" ,hash_commit , commitGet(c, "message"));
        }else{
            printf("%s \n", hash_commit);
        }
        if (commitGet(c, "predecessor")!=NULL){
            hash_commit = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(hash_commit));
        }else{
            c = NULL;
        }
    }
}

*/
//Fonction non segmenté avec des conditions pour eviter les segmentations fault
/* 8.6- parcourt la branche appelée
branch, et pour chacun de ses commits, affiche son hash et son message descriptif */
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
    // On récupère le hash du commit de la branche
    char * path = malloc(6 + strlen(branch));
    strcat(path, ".refs/");
    strcat(path, branch);
    // On charge le commit à partir du chemin .refs/branch
    Commit* c = ftc(path);
    // On parcourt les commits de la branche
    while (c != NULL){
        // On affiche le hash et le message du commit
        if (commitGet(c, "message")!=NULL){
            printf("%s -> %s \n" ,hash_commit , commitGet(c, "message"));
        }else{
            printf("%s \n", hash_commit);
        }
        // On passe au commit précédent si disponible
        if (commitGet(c, "predecessor")!=NULL){
            hash_commit = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(hash_commit));
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
    char* hash_commit = getRef(branch);
    Commit* c = ftc(hashToPathCommit(hash_commit)); 
    while (c != NULL){
        insertFirst(L, buildCell(hash_commit)); 
        if (commitGet(c, "predecessor")!=NULL){
            hash_commit = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(hash_commit)); 
        }else{
            c = NULL;
        }
    }
    return L;
}
/* 8.6 - construit et retourne une liste
chaînée contenant le hash de tous les commits de la branche */
List* branchList(char* branch) {
    // On initialise une liste vide
    List* L = initList();

    // récupération du hash du dernier commit de la branche
    char* commit_hash = getRef(branch);

    // On charge le commit correspondant au hash récupéré
    Commit* c = ftc(hashToPathCommit(commit_hash)); 

    // On parcourt tous les commits de la branche
    while (c != NULL) {
        // On insère le hash du commit actuel en tête de la liste
        insertFirst(L, buildCell(commit_hash)); 

        // Si le commit actuel a un prédécesseur on passe au commit précédent
        if (c != NULL && commitGet(c, "predecessor") != NULL) {
            commit_hash = commitGet(c, "predecessor");
            c = ftc(hashToPathCommit(commit_hash)); 
        } else {
            c = NULL;
        }
    }
    // On retourne la liste des hash de commits de la branche
    return L;
}


/* 8.7 - renvoie la liste des hash des commits de
toutes les branches (sans répétition) */
List* getAllCommits() {
    // On initialise une liste vide pour stocker les hash des commits
    List* L = initList();
    // On récupère la liste des fichiers dans le répertoire ".refs"
    List* content = listdir(".refs");
    // On parcourt tous les fichiers du répertoire ".refs"
    for (Cell* ptr = *content; ptr != NULL; ptr = ptr->next) {
        // On ignore les fichiers cachés (commençant par un point)
        if (ptr->data[0] == '.') { continue; }
        // On récupère la liste des hash de commits de la branche courante
        List* list = branchList(ptr->data);
        // On parcourt la liste des hash de commits de la branche courante
        Cell* cell = *list;
        while (cell != NULL) {
            // Si le hash du commit n'est pas déjà présent dans la liste globale, on l'ajoute
            if (searchList(L, cell->data) == NULL) { 
                insertFirst(L, buildCell(cell->data));
            }
            // On passe au commit suivant de la branche courante
            cell = cell->next; 
        }
    }
    // On retourne la liste des hash des commits de toutes les branches (sans répétition)
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