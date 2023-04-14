#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/Exercice2.h"

//Exercice 2 : 


/*Initialise une liste vide*/
List* initList() {
    List* res = malloc(sizeof(List));
    *res = NULL;
    return res;
}

/*Alloue et retourne une cellule de la liste*/
List buildCell(char* ch) {
    List new_cell = malloc(sizeof(Cell));
    if (new_cell == NULL) {
        printf("erreur lors de l'allocution de la cellule BUILDCELL.\n");
        exit(1);
    }
    new_cell->data = malloc(strlen(ch) + 1);
    if (new_cell->data == NULL) {
        printf("erreur lors de l'allocution du data de la cellule BUILDCELL.\n");
        exit(1);
    }
    strcpy(new_cell->data, ch);
    new_cell->next = NULL;
    return new_cell;
}

/*Ajoute un élément en tête d'une liste*/
void insertFirst(List* L, List C) {
    if (C == NULL) {
        printf("Erreur je ne peux pas inserer de cellule null .\n");
        return;
    }
    C->next = (*L);
    (*L) = C;
}

/*Retourne la chaine de caracteres que c représente*/
char* ctos(List c) {
    return c->data;
}

/*Transforme une liste en une chaine de caractere avec le format : str1|str2|str3 */
char* ltos(List* L){ 
    if (*L == NULL){
        return ""; 
    }
    char* ch = malloc(2*sizeof(char)); 
    List ptr = *L;
    while (ptr != NULL){
        strcat(ch, ptr->data); 
        ptr = ptr->next;
        if (ptr != NULL) {
            strcat(ch, "|");
        }
    }
    return ch; 
}

/*Renvoie le ieme élément d'une liste*/
List listGet(List* L, int i) {
    List curr = *L;
    int indice = 0;

    while (curr != NULL) {
        if (indice == i){ break;}

        curr = curr->next;
        indice++;
    }
    if(curr == NULL){
        printf("Index out of range !\n");
    }
    return curr;
}

/*Recherche un élément dans une liste à partir de son contenu et renoie la
  reference vers lui ou NULL s'il n'est pas dans la liste */
List searchList(List* L, char* str) {
    List curr = *L;
    while (curr != NULL) {
        if (strcmp(curr->data, str) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/*Fonction annexe pour stol*/
void insertLast(List* L, List C) {
    List curr = (*L);
    if(curr == NULL){
        curr = C;
    }else{
        while(curr->next){
            curr = curr->next;
        }
        curr->next = C;
    }
    C->next = NULL;
}

/*transformer une chaîne de caractères représentant une liste en une liste chaînée.*/
List* stol(char* s) {
    List* L = initList();
    char * copie_s_debug=strdup(s);
    char * tok = strtok(copie_s_debug, "|");
    while (tok != NULL) {
        List new_cell = buildCell(tok);
        insertLast(L, new_cell);
        tok = strtok(NULL, "|");
    }
    return L;
}

//Fonction annexe ajoute pour debug:
void affiche(List* L) {
    List curr = (*L);
    while (curr != NULL) {
        printf("%s \n", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

/*Ecrire une liste dans le fichier qui se trouve à path*/
void ltof(List* L, char* path){
    FILE *fp = fopen(path, "w"); 
    if (fp != NULL){
        fputs(ltos(L), fp);
        fclose(fp);
    }
}

/*Lire une liste enregistree dans une fichier*/
List* ftol(char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return NULL;
    }
    char buffer[1024];
    fgets(buffer, 1024, f);
    fclose(f);
    buffer[strcspn(buffer, "\n")] = 0; // supprimer le retour à la ligne
    return stol(buffer);
}


