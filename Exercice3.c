#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "main.h"
#define EX3

typedef struct List {
    char **names;
    int size;
} List;

//J'ai du rajouter cette fonction car les indications au préprocesseur ifndef endif ne fonctionnait pas pour les main (posaient des problemes à la compilation)
int hashFile(char* source, char* dest){
    char buffer[256];
    sprintf(buffer,"cat %s|sha256sum>%s",source,dest);
    system(buffer);
    return 1;
}

List* listdir(char* root_dir) {
    DIR *dp = opendir(root_dir);
    struct dirent *ep;
    List *list = malloc(sizeof(List));
    list->size = 0;
    list->names = NULL;

    if (dp != NULL) {
        while ((ep = readdir(dp)) != NULL) {
            list->names = realloc(list->names, (list->size + 1) * sizeof(char *));
            list->names[list->size] = strdup(ep->d_name);
            list->size++;
        }
        closedir(dp);
    }
    return list;
}

int file_exists(char *file) {
    List *list = listdir(".");
    for (int i = 0; i < list->size; i++) {
        if (strcmp(list->names[i], file) == 0) {
            return 1;
        }
    }
    return 0;
}

void cp(char *dest, char *src) {
    if (file_exists(src)) {
        FILE *source = fopen(src, "r");
        FILE *destination = fopen(dest, "w");
        char line[1024];

        while (fgets(line, sizeof(line), source) != NULL) {
            fputs(line, destination);
        }

        fclose(source);
        fclose(destination);
    } else {
        printf("Le fichier source n'existe aps\n");
    }
}


char* hashToPath(char* hash) {
    char *chemin = malloc(strlen(hash) + 2);
    strncpy(chemin, hash, 2);
    chemin[2] = '/';
    strcpy(chemin + 3, hash + 2);
    return chemin;
}



void blobFile(char* file) {
    if (!file_exists(file)) {
        printf("Le fichier n'existe pas\n");
        return;
    }

    char hash[256];
    hashFile(file,hash);
    printf("Le hash du fichier file passé en paramètre est: %s",file);
    char *hash_path = hashToPath(hash);

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "mkdir -p .git/objects/%.2s", hash); //Plus sécurisé que sprintf
    system(buffer);

    char buffer2[256];
    snprintf(buffer2, sizeof(buffer2), "cp %s .git/objects/%s", file, hash_path);
    system(buffer2);

    free(hash_path);
}

int main() {
    char *root_dir = ".";

    // listdir:
    printf("Liste des fichiers dans le répertoire courant %s: [TEST LISTDIR]\n", root_dir);
    List *list = listdir(root_dir);
    for (int i = 0; i < list->size; i++) {
        printf("%s\n", list->names[i]);
    }

    // file_exists:
    char *test = "test.txt";
    if (file_exists(test)) {
        printf("Le fichier %s existe.\n", test);
    } else {
        printf("Le fichier %s n'existe pas [TEST FILE_EXISTS]\n", test);
    }

    // cp
    char *src_file = "test.txt";
    char *dest_file = "test_copy.txt";
    cp(dest_file, src_file);
    printf("Fichier copié de %s à %s [TEST CP]\n", src_file, dest_file);

    // hashToPath
    char *hash = "5e884898da28047151d0e56f8dc6292773603ddc";
    char *hash_path = hashToPath(hash);
    printf("Chemin pour le hash [TEST HASHTOPATH] %s: %s\n", hash, hash_path);
    free(hash_path);

    // blobFile
    char *file_to_blob = "test.txt";
    blobFile(file_to_blob);
    printf("Instantané du fichier %s créé [TEST BLOBFILE]\n", file_to_blob);

    return 0;
}
