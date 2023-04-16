#include "../headers/Exercice2.h"
#include <assert.h>
#include <string.h>

int main() {
    // Initialisation d'une liste vide
    List* L = initList();

    // Ajout de quelques éléments à la liste
    insertFirst(L, buildCell("one"));
    insertFirst(L, buildCell("two"));
    insertFirst(L, buildCell("three"));

    // Affichage de la liste sous forme de chaîne de caractères
    assert(strcmp("three", (*L)->data) == 0);
    printf("%s\n", ltos(L));

    // Récupération d'un élément de la liste à un indice donné
    List c = listGet(L, 1);
    //printf("L'élément à l'indice 1 est : %s\n", ctos(c));
    List d = listGet(L, 2);
    //printf("L'élément à l'indice 2 est : %s\n", ctos(d));
    
    // Recherche d'un élément dans la liste
    List found = searchList(L, "three");
    if (found != NULL) {
        printf("L'élément 'three' a été trouvé à l'adresse %p\n", found);
    } else {
        printf("L'élément 'three' n'a pas été trouvé dans la liste\n");
    }
    List found1 = searchList(L, "world");
    if (found1 != NULL) {
        printf("L'élément 'world' a été trouvé à l'adresse %p\n", found1);
    } else {
        printf("L'élément 'world' n'a pas été trouvé dans la liste\n");
    }
    
    // Conversion d'une chaîne de caractères en liste
    char* s = "foo|bar|baz";
    List* L2 = stol(s);
    printf("La liste construite à partir de la chaîne '%s' est : %s\n", s, ltos(L2));
    // Lecture d'une liste à partir d'un fichier
    List* L3 = ftol("TestFiles/test.txt");
    if (L3 != NULL) {
        printf("La liste lue dans le fichier est : %s\n", ltos(L3));
    }
    // Libération de la mémoire allouée
    free(L);
    free(c);
    free(d);
    free(found);
    free(found1);
    free(L2);
    free(L3);
    return 0;
}
