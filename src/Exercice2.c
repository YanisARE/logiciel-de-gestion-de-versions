
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/Exercice2.h"

#define MAIN2
//Exercice 2 : 



List* initList() {
    List l = malloc(sizeof(Cell));
    l->next = NULL;
    List* res = malloc(sizeof(List));
    *res = l;
    return res;
}


List buildCell(char* ch) {
    List new_cell = malloc(sizeof(Cell));
    new_cell->data = malloc(strlen(ch) + 1);
    strcpy(new_cell->data, ch);
    new_cell->next = NULL;
    return new_cell;
}

void insertFirst(List* L, List C) {
    C->next = (*L)->next;
    (*L)->next = C;
}
char* ctos(List c) {
    return c->data;
}
char* ltos(List* L) {
    List chaine_curr = (*L)->next;
    char* resultat = malloc(1); // allocation d'un caractère nul pour la chaîne vide
    resultat[0] = '\0';
    while (chaine_curr != NULL) {
        char* chaine = ctos(chaine_curr);
        resultat = realloc(resultat, strlen(resultat) + strlen(chaine) + 2);
        strcat(resultat, chaine);
        strcat(resultat, "|");
        chaine_curr = chaine_curr->next;
    }
    return resultat;
}

List listGet(List* L, int i) {
    List curr = (*L)->next;
    int indice = 0;

    while (curr != NULL && indice < i) {
        curr = curr->next;
        indice++;
    }

    return curr;
}

List searchList(List* L, char* str) {
    List curr = (*L)->next;
    while (curr != NULL) {
        if (strcmp(curr->data, str) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

List* stol(char* s) {
    List* L = initList();
    char* tok = strtok(s, "|"); /*strtok permet de découper une chaîne de caractères en morceaux plus petits, ici découpé selon le caractère "|" */
    while (tok != NULL) {
        insertFirst(L, buildCell(tok));
        tok = strtok(NULL, "|"); //Ici permet d'effectuer une duexième fois le découpage sur la même chaine car sans NULL il ne le ferais pas apparemment
    
    }
    return L;
}

List* ftol(char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier");
        return NULL;
    }
    char buffer[1024];
    fgets(buffer, 1024, f);
    fclose(f);
    buffer[strcspn(buffer, "\n")] = 0; // supprimer le retour à la ligne
    return stol(buffer);
}


