#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/Exercice3.h"
#include <sys/stat.h>

#define EX3_MAIN

/*Prend en parametre une PATH et renvoie une liste contenant le nom des fichiers et repertoire qui s'y trouve*/
List * listdir ( char * root_dir ){
    DIR *dp;
    struct dirent *ep;
    List * L = initList ();
    *L = NULL ;
    List temp_cell ;
    dp = opendir ( root_dir );
    if (dp != NULL ){
        while (( ep = readdir (dp)) != NULL ){
            temp_cell = buildCell (ep -> d_name );
            insertFirst (L, temp_cell );
            List ptr = *L;
            while (ptr != NULL ){
                ptr = ptr -> next ;
            }
        }
    ( void ) closedir (dp);
    }
    else{
        perror ("Impossible d'ouvrir le repertoire");
    return NULL ;
    }
    printf("L  dans listdir vaut :%p\n",L);
    return L;
}
/*Fonction annexe de file_exists qui permet de free une structure List:*/
void freeList(List* L) {
    List current = *L;
    List next;

    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    *L = NULL;
}


/*Retourne 1 si le fichier file existe dans le repertoire courant 0 sinon*/
struct stat st = {0};

int file_exists ( char * file ) {
    struct stat buffer ;
    if (stat ( file , & buffer ) == 0) return 1;
    else{
        return 0;
    }
}
/*Copie le contenu d'un fichier vers un autre, en faisant une lettre*/
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


