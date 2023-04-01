#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAINC


//1.3:
int hashFile(char* source, char* dest){
    char buffer[256];
    sprintf(buffer,"cat %s|sha256sum>%s",source,dest);
    system(buffer);
    return 1;
}

//1.4:
char* sha256file(char* file){   
    static char template [] = "/tmp/myfileXXXXXXX" ;
    char fname [1000];
    strcpy ( fname , template ) ;
    int val_retour=hashFile(file, fname);
    printf("Le hachage s'est bien passee : %d\n",val_retour);
    char hash[256];
    FILE *f=fopen(fname,"r");
    fgets(hash,256,f);
    fclose(f);
    unlink(fname);
    char *resultat = malloc(sizeof(hash));
    strcpy(resultat, hash);
    return resultat;
}


#ifndef MAINC
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

    return 0;
}
#endif