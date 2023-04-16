#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//1.3 - Fonction qui calcule le hash du contenu d'un fichier source
// et l'écrit dans un fichier de destination.
int hashFile(char* source, char* dest){
    // Buffer pour stocker la commande
    char buffer[256];

    // Créer la commande pour calculer le hash SHA-256 du fichier source et le stocker dans un fichier temporaire
    sprintf(buffer,"cat %s|sha256sum>%sTestFiles/%s",source, dest, "hash.txt");

    // Exécuter la commande
    system(buffer);

    return 1;
}


//1.4:- Fonction qui calcule le hash du contenu d'un fichier en utilisant
// la commande sha256sum de Linux et retourne une chaîne de caractères représentant le hash.
char *sha256file(char *file) {
    // Créer un nom de fichier temporaire
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[1000];
    strcpy(fname, template);

    // Générer un fichier temporaire unique
    int fd = mkstemp(fname);

    // Construire la commande pour calculer le hash SHA-256 du fichier
    char cmd[20000];
    sprintf(cmd, "cat %s | sha256sum > %s", file, fname);

    // Exécuter la commande
    system(cmd);

    // Allouer de l'espace pour stocker le hash
    char *hash = (char *)malloc(65 * sizeof(char));

    // Lire le hash du fichier temporaire
    FILE *f = fopen(fname, "r");
    fgets(hash, 65, f);
    hash[64] = '\0';

    // Supprimer le fichier temporaire
    char command[10000];
    sprintf(command, "rm %s", fname);
    system(command);

    return hash;
}

/* fonctionnait au debut puis nous a cree un probleme a l'exercice 6 donc on a pris celle de la correction 
char* sha256file(char* file){   
    static char template [] = "TestFiles/myfileXXXXXXX" ;
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
*/



