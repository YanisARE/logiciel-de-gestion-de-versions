#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "main.h"
#include "Exercice2.h"
#include "Exercice3.h"
#include "Exercice4.h"
#include "Exercice5.h"


int main() {
    char* source_file = "test.txt";
    char* hash_destination = "hash.txt";

    // Test de la fonction hashFile
    int hash_result = hashFile(source_file, hash_destination);
    if (hash_result) {
        printf("Le fichier a ete hache avec succes. Le resultat est dans le fichier %s.\n", hash_destination);
    } else {
        printf("Erreur lors du hachage du fichier.\n");
    }

    // Test de la fonction sha256file
    char* sha256_result = sha256file(source_file);
    if (sha256_result) {
        printf("Le hachage SHA256 du fichier %s est : %s\n", source_file, sha256_result);
        free(sha256_result);
    } else {
        printf("Erreur lors du calcul du hachage SHA256 du fichier.\n");
    }
    //Test Exercice2 : 

    //system("ls");
    List2* L = initList();
    
    insertFirst(L, buildCell("one"));
    insertFirst(L, buildCell("two"));
    insertFirst(L, buildCell("three"));
    
    printf("L : %s\n", ltos(L));
    
    Cell* Cellule = listGet(L, 1);
    printf("Cellule : %s\n", ctos(Cellule));

    //Test Exercice 3 : 
    char *root_dir = ".";

    // listdir:
    printf("Liste des fichiers dans le répertoire courant %s: [TEST LISTDIR]\n", root_dir);
    List2 *list2 = listdir(root_dir);
    for (int i = 0; i < list2->size; i++) {
        printf("%s\n", list2->names[i]);
    }

    // file_exists:
    char *test = "/TestFiles/test.txt";
    if (file_exists(test)) {
        printf("Le fichier %s existe.\n", test);
    } else {
        printf("Le fichier %s n'existe pas [TEST FILE_EXISTS]\n", test);
    }

    // cp
    char *src_file = "/TestFiles/test.txt";
    char *dest_file = "/TestFiles/test_copy.txt";
    cp(dest_file, src_file);
    printf("Fichier copié de %s à %s [TEST CP]\n", src_file, dest_file);

    // hashToPath
    char *hash = "5e884898da28047151d0e56f8dc6292773603ddc";
    char *hash_path = hashToPath(hash);
    printf("Chemin pour le hash [TEST HASHTOPATH] %s: %s\n", hash, hash_path);
    free(hash_path);

    // blobFile
    char *file_to_blob = "/TestFiles/test.txt";
    blobFile(file_to_blob);
    printf("Instantané du fichier %s créé [TEST BLOBFILE]\n", file_to_blob);

    //TestExercice 4 : 
    
    WorkTree *wt = initWorkTree();
    appendWorkTree(wt, "file1.txt", "hash1", 0644);
    appendWorkTree(wt, "file2.txt", "hash2", 0755);

    char *wt_str = wtts(wt);
    printf("WorkTree:\n%s\n", wt_str);

    int ret = wttf(wt, "TestFiles\\worktree.txt");
    if (ret == 0) {
        printf("WorkTree ecrit dans worktree.txt\n");
    }

    WorkTree *wt_read = ftwt("TestFiles\\worktree.txt");
    char *wt_read_str = wtts(wt_read);
    printf("WorkTree lu depuis le fichier:\nTestFiles/%s\n", wt_read_str);

    free(wt_str);
    free(wt_read_str);
    
    /*
    //Test Exercice 5 : 
    
    // Test pour la question 1
    char* hash = "a1b2c3d4e5f6";
    char* path = hashToFile(hash);
    printf("Le chemin associé à %s est : %s\n", hash, path);

    // Test pour la question 2
    WorkTree* wt = initWorkTree();
    appendWorkTree(wt, "test1.txt", 0, NULL);
    appendWorkTree(wt, "test2.txt", 0, NULL);
    char* saved = saveWorkTree(wt, "/tmp");
    printf("Le hash associé à l'arbre de travail sauvegardé est : %s\n", saved);

    // Test pour la question 3
    WorkTree* new_wt = initWorkTree();
    restoreWorkTree(new_wt, "/tmp");
    printf("Les fichiers restaurés sont : \n");
    for (int i=0; i<new_wt->n; i++){
        printf("%s\n", new_wt->tab[i].name);
    }
    */
    
    return 0;
}