#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/Exercice2.h"

//Exercice 2 : 


/*Initialise une liste vide*/
List* initList() {
    List l = malloc(sizeof(Cell));
    l->next = NULL;
    List* res = malloc(sizeof(List));
    *res = l;
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
    C->next = (*L)->next;
    (*L)->next = C;
}

/*Retourne la chaine de caracteres qu'elle représente*/
char* ctos(List c) {
    return c->data;
}

/*Transforme une liste en une chaine de caractere avec le format : str1|str2|str3 */
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

/*Renvoie le ieme élément d'une liste*/
List listGet(List* L, int i) {
    List curr = (*L)->next;
    int indice = 0;

    while (curr != NULL && indice < i) {
        curr = curr->next;
        indice++;
    }

    return curr;
}

/*Recherche un élément dans une liste à partir de son contenu et renoie la
  reference vers lui ou NULL s'il n'est pas dans la liste */
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

/*transformer une chaîne de caractères représentant une liste en une liste chaînée.*/
List* stol(char* s) {
    //printf("rentre dans stol\n");
    List* L = initList(); //Ok
    //printf("initlist stol\n");
    char * copie_s_debug=strdup(s);
    char* tok = strtok(copie_s_debug, "|");
    //printf("strtok stol") ;
    while (tok != NULL) {
        //printf("rentre dans while stol");
        List new_cell = buildCell(tok);
        //printf("rentre apres buildcell");
        insertFirst(L, new_cell);
        //printf("apres insertFirst (buildCell) ");
        tok = strtok(NULL, "|"); 
        //printf("apres strtok");
    }
    
    return L;
}

//Fonction annexe ajoute pour debug:
void affiche(List* L) {
    List curr = (*L)->next;
    while (curr != NULL) {
        printf("%s ", curr->data);
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
        printf("Impossible d'ouvrir le fichier");
        return NULL;
    }
    char buffer[1024];
    fgets(buffer, 1024, f);
    fclose(f);
    buffer[strcspn(buffer, "\n")] = 0; // supprimer le retour à la ligne
    return stol(buffer);
}