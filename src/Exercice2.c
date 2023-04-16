#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../headers/Exercice2.h"

//Exercice 2 : 


/*2.1:-Initialise une liste vide
*/
List* initList() {
    List* res = malloc(sizeof(List));
    *res = NULL;
    return res;
}

/*2.2:-Alloue et retourne une cellule de la liste
*/
List buildCell(char* ch) {
    List new_cell = malloc(sizeof(Cell));
    if (new_cell == NULL) {
        printf("erreur lors de l'allocution de la cellule BUILDCELL.\n");
        exit(1);
    }
    new_cell->data = malloc(strlen(ch) + 1); // Alloue de la mémoire pour la chaîne de caractères dans la cellule
    if (new_cell->data == NULL) {
        printf("erreur lors de l'allocution du data de la cellule BUILDCELL.\n");
        exit(1);
    }
    strcpy(new_cell->data, ch); // Copie la chaîne de caractères dans la cellule
    new_cell->next = NULL; // Initialise le pointeur next à NULL
    return new_cell;
}

/*2.3:-Ajoute un élément en tête d'une liste
*/
void insertFirst(List* L, List C) {
    if (C == NULL) {
        printf("Erreur je ne peux pas inserer de cellule null .\n");
        return;
    }
    C->next = (*L); // Fait pointer la cellule insérée vers l'ancienne première cellule
    (*L) = C; // Met à jour le pointeur de la liste pour qu'il pointe vers la nouvelle première cellule
}

/*2.4:-Retourne la chaine de caracteres que c représente
*/
char* ctos(List c) {
    return c->data; // Retourne la chaîne de caractères stockée dans la cellule
}

/*Transforme une liste en une chaine de caractere avec le format : str1|str2|str3 */
char* ltos(List* L){ 
    if (*L == NULL){
        return ""; 
    }
    char* ch = malloc(2*sizeof(char)); 
    List ptr = *L;
    while (ptr != NULL){
        strcat(ch, ptr->data); // Concatène la chaîne de caractères de la cellule actuelle à ch
        ptr = ptr->next; // Passe à la cellule suivante
        if (ptr != NULL) {
            strcat(ch, "|"); // Ajoute un séparateur si ce n'est pas la dernière cellule
        }
    }
    return ch; 
}

/*2.5:-Renvoie le ieme élément d'une liste NULL si l'indice est hors limites
*/
List listGet(List* L, int i) {
    List curr = *L;
    int indice = 0;

    while (curr != NULL) {
        if (indice == i){ 
            break;
        }

        curr = curr->next;  // Passe à la cellule suivante
        indice++;
    }
    if(curr == NULL){
        printf("Index out of range !\n");
    }
    return curr; 
}

/*2.6:-Recherche un élément dans une liste à partir de son contenu et renvoie la
    reference vers lui ou NULL s'il n'est pas dans la liste 
*/
List searchList(List *L, char *str) {
    List curr = *L;
    while (curr != NULL) {
        if (strcmp(curr->data, str) == 0) {
            return curr; // Retourne la cellule si la chaîne de caractères correspond
        }
        curr = curr->next; // Passe à la cellule suivante
    }
    return NULL; // Retourne NULL si la chaîne de caractères n'a pas été trouvée
}

/*Fonction annexe pour stol*/
void insertLast(List *L, List C) {
    List curr = (*L);
    if (curr == NULL) {
        curr = C;
    } else {
        while (curr->next) {
            curr = curr->next; // Passe à la cellule suivante
        }
        curr->next = C; // Ajoute la nouvelle cellule à la fin de la liste
    }
    C->next = NULL;
}

/*2.7:-transformer une chaîne de caractères représentant une liste en une liste chaînée.*/
List *stol(char *s) {
    List *L = initList(); // Initialise une nouvelle liste vide
    char *copie_s_debug = strdup(s); // Crée une copie de la chaîne de caractères pour ne pas la modifier
    char *tok = strtok(copie_s_debug, "|"); // Récupère le premier élément de la chaîne de caractères (séparateur "|")
    
    while (tok != NULL) {
        List new_cell = buildCell(tok); // Crée une nouvelle cellule avec la chaîne de caractères trouvée
        insertLast(L, new_cell); // Insère la nouvelle cellule à la fin de la liste
        tok = strtok(NULL, "|"); // Récupère le prochain élément de la chaîne de caractères (séparateur "|")
    }
    return L; // Retourne la liste chaînée créée
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

/*2.8:-Ecrire une liste dans le fichier qui se trouve à path*/
void ltof(List* L, char* path) {
    FILE *fp = fopen(path, "w"); // Ouvre le fichier en mode écriture
    
    if (fp != NULL) {
        fputs(ltos(L), fp); // Écrit la chaîne de caractères représentant la liste dans le fichier
        fclose(fp); // Ferme le fichier
    }
}

/*2.8-Lire une liste enregistree dans une fichier*/
List *ftol(char* path) {
    FILE *f = fopen(path, "r"); // Ouvre le fichier en mode lecture
    
    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return NULL;
    }
    
    char buffer[1024];
    fgets(buffer, 1024, f); // Lit une ligne du fichier et la stocke dans buffer
    fclose(f); // Ferme le fichier
    buffer[strcspn(buffer, "\n")] = 0; // Supprime le retour à la ligne à la fin de la chaîne de caractères
    
    return stol(buffer); // Transforme la chaîne de caractères en liste chaînée et retourne la liste
}


