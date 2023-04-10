#include "../headers/Exercice5.h"

int main(){
    //Test Exercice 5 : 
    
    // Test pour la question 1
    char* hash = "a1b2c3d4e5f6";
    char* path = hashToFile(hash);
    printf("Le chemin associé à %s est : %s\n", hash, path);
    */

    // Test pour la question 2
    WorkFile * test=createWorkFile("Test");
    WorkTree* wt = initWorkTree();
    appendWorkTree(wt, "TestFiles/test1.txt", 0, test->mode); //c'est <f->mode pas wt 
    appendWorkTree(wt, "TestFiles/test2.txt", 0, test->mode);
    char* saved = saveWorkTree(wt, "/tmp");
    printf("Le hash associé à l'arbre de travail sauvegardé est : %s\n", saved);

    // Test pour la question 3
    WorkTree* new_wt = initWorkTree();
    restoreWorkTree(new_wt, "/tmp");
    printf("Les fichiers restaurés sont : \n");
    for (int i=0; i<new_wt->n; i++){
        printf("%s\n", new_wt->tab[i].name);
    }
    return 0;
}