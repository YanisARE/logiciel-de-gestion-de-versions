
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAIN2
//Exercice 2 : 

typedef struct cell {
    char * data ;
    struct cell * next ;
} Cell ;
typedef Cell * List ;

List* initList() {
    List l = malloc(sizeof(Cell));
    l->next = NULL;
    List* res = malloc(sizeof(List));
    *res = l;
    return res;
}


Cell* buildCell(char* ch) {
    Cell* new_cell = malloc(sizeof(Cell));
    new_cell->data = malloc(strlen(ch) + 1);
    strcpy(new_cell->data, ch);
    new_cell->next = NULL;
    return new_cell;
}

void insertFirst(List* L, Cell* C) {
    C->next = (*L)->next;
    (*L)->next = C;
}
char* ctos(Cell* c) {
    return c->data;
}
char* ltos(List* L) {
    Cell* chaine_curr = (*L)->next;
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

Cell* listGet(List* L, int i) {
    Cell* curr = (*L)->next;
    int indice = 0;

    while (curr != NULL && indice < i) {
        curr = curr->next;
        indice++;
    }

    return curr;
}

Cell* searchList(List* L, char* str) {
    Cell* curr = (*L)->next;
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


//#ifndef MAIN2
int main(){
    //system("ls");
    List* L = initList();
    
    insertFirst(L, buildCell("one"));
    insertFirst(L, buildCell("two"));
    insertFirst(L, buildCell("three"));
    
    printf("L : %s\n", ltos(L));
    
    Cell* Cellule = listGet(L, 1);
    printf("Cellule : %s\n", ctos(Cellule));
    
    return 0;
}
//#endif