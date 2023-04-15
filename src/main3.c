#include "../headers/Exercice3.h"


int main() {
    char *root_dir = ".";

    // listdir:
    printf("Liste des fichiers dans le répertoire courant %s  : [TEST LISTDIR]\n", root_dir);
    List *list = listdir(root_dir);
    printf("Affichage list main3.c:\n");
    affiche(list);
    while (*list!=NULL){
        //printf("data:%s\n",(*list)->data);
        (*list)=(*list)->next;
    }
    
    // file_exists:
    printf("Test file_exists:\n");
    char *test = "TestFiles/test.txt";
    if (file_exists(test)) {
        printf("Le fichier %s existe.\n", test);
    } else {
        printf("Le fichier %s n'existe pas [TEST FILE_EXISTS]\n", test);
    }
    
    // cp
    char *src_file = "TestFiles/test.txt";
    char *dest_file = "TestFiles/test_copy.txt";
    cp(dest_file, src_file);
    printf("Fichier copié de %s à %s [TEST CP]\n", src_file, dest_file);
    
    // hashToPath
    char *hash = "5e884898da28047151d0e56f8dc6292773603ddc";
    char *hash_path = hashToPath(hash);
    printf("Chemin pour le hash [TEST HASHTOPATH] %s: %s\n", hash, hash_path);
    free(hash_path);
    
    // blobFile
    char *file_to_blob = "TestFiles/test.txt";
    blobFile(file_to_blob);
    printf("Instantané du fichier %s créé [TEST BLOBFILE]\n", file_to_blob);
    
    return 0;
}