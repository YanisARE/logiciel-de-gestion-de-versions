#include "../headers/Exercice2.h"

int main(){
    //system("ls");
    List* L = initList();
    
    insertFirst(L, buildCell("one"));
    insertFirst(L, buildCell("two"));
    insertFirst(L, buildCell("three"));
    
    printf("L : %s\n", ltos(L));
    
    List Cellule = listGet(L, 1);
    printf("Cellule : %s\n", ctos(Cellule));
    
    return 0;
}
