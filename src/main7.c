#include "../headers/Exercice7.h"


int main() {
    // Initialisation des références
    initRefs();

    // Création ou mise à jour d'une référence
    createUpdateRef("example", "123456789abcdef");

    // Suppression d'une référence
    // deleteRef("example");

    // Récupération du hash d'une référence
    char *hash = getRef("example");
    printf("Hash of example ref: %s\n", hash);

    // Création d'un fichier
    createFile("test.txt");

    // Ajout d'un fichier au WorkTree
    myGitAdd("test.txt");

    // Création d'un commit
    Commit *new_commit = createCommit("abcdef123456789", 1);

    // Modification d'un commit
    commitSet(new_commit, "message", "Mon premier commit");

    // Récupération d'une valeur d'un commit
    char *commit_message = commitGet(new_commit, "message");
    printf("Commit message: %s\n", commit_message);

    // Commit des modifications
    myGitCommit("master", "Mon premier commit");

    // Nettoyage
    free(hash);

    return 0;
}


