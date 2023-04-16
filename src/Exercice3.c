#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "../headers/Exercice3.h"
#include <sys/stat.h>

#define EX3_MAIN

/*3.1-Prend en parametre une PATH et renvoie une liste contenant le nom des fichiers et repertoire qui s'y trouve*/
List * listdir ( char * root_dir ){
    DIR *dp;
    struct dirent *ep;
    List * L = initList ();
    *L = NULL ;
    List temp_cell ;
    dp = opendir ( root_dir ); //On ouvre un pointeur vers le repertoire de chemin root_dir passe en paramètre de la fonction
    if (dp != NULL ){
        while (( ep = readdir (dp)) != NULL ){
            temp_cell = buildCell (ep -> d_name ); // Crée une nouvelle cellule avec le nom du fichier/répertoire
            insertFirst (L, temp_cell ); // Insère la nouvelle cellule en début de liste
            List ptr = *L;
            while (ptr != NULL ){
                ptr = ptr -> next ;
            }
        }
    (void) closedir (dp); // Ferme le répertoire
    }
    else{
        perror ("Impossible d'ouvrir le repertoire");
    return NULL ;
    }
    //printf("L  dans listdir vaut :%p\n",L);
    return L;
}

/*Fonction annexe de file_exists qui permet de free une structure List:*/
void freeList(List* L) {
    List current = *L;
    List tmp;

    while (current != NULL) {
        tmp = current->next;
        free(current->data);// Libère la donnée stockée dans la cellule
        free(current); //Libère la cellule elle même
        current = tmp;
    }
    *L = NULL;
}


/*3.2-Retourne 1 si le fichier file existe dans le repertoire courant 0 sinon
*/
struct stat st = {0};

int file_exists ( char * file ) {
    struct stat buffer ;
    if (stat ( file , & buffer ) == 0) return 1;
    else{
        return 0;
    }
}
/*3.3-Copie le contenu d'un fichier vers un autre, en faisant une lettre*/
void cp(char *dest, char *src) {
    if (file_exists(src)) {
        FILE *source = fopen(src, "r"); //On ouvre un pointeur FILE vers le fichier source
        FILE *destination = fopen(dest, "w"); //On ouvre un pointeur FILE vers le fichier destination
        char ligne[1024];

        while (fgets(ligne, sizeof(ligne), source) != NULL) { //On parcourt le fichier ligne par ligne
            fputs(ligne, destination); // Copie la ligne du fichier source vers le fichier de destination
        }

        fclose(source); //On ferme le pointeur source
        fclose(destination); //On ferme le pointeur destination
    } else {
        printf("Le fichier source n'existe aps\n");
    }
}

/*3.4-retourne le chemin d’un fichier à partir de son hash*/
char* hashToPath(char* hash) {
    if (hash == NULL) {
        return NULL;
    }
    char *chemin = malloc(strlen(hash) + 2); //On allout de l'espace memoire pour le chemin + le symbole "/"
    strncpy(chemin, hash, 2); //On copie les deux premiers carcatère du hash dans le chemin 
    chemin[2] = '/'; //On place le caractère "/"
    strcpy(chemin + 3, hash + 2); //On met à parti du troisième caractère dans chemin le hash 
    return chemin;
}

/*3.5-enregistre un instantané du fichier donné en entrée*/
void blobFile(char* file) {
    if (!file_exists(file)) {
        printf("Le fichier n'existe pas\n");
        return;
    }

    char hash[256];
    hashFile(file,hash); // On calcule le hash du fichier
    printf("Le hash du fichier file passé en paramètre est: %s",file);
    char *hash_path = hashToPath(hash); //On creer le chemin du hash

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "mkdir -p .git/objects/%.2s", hash); // Crée le répertoire pour stocker les objets Git en utilisant les deux premiers caractères du hash (%.2s) avec l'option -p qui permet de créer tous les répertoires qui n'existerait pas
    system(buffer);

    char buffer2[256];
    snprintf(buffer2, sizeof(buffer2), "cp %s .git/objects/%s", file, hash_path); // Copie le fichier dans le répertoire des objets Git en utilisant son hash comme chemin
    system(buffer2); 

    free(hash_path);
}


