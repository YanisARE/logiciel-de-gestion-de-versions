#include "../headers/main.h"


int main() {
    char* source_file = "TestFiles/test.txt";
    char* hash_destination = "TestFiles/hash.txt";

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