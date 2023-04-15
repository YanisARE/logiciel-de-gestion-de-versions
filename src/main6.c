#include "../headers/Exercice6.h"


int main() {
    // Création d'un commit
    Commit* monCommit = initCommit();
    
    // Ajout des données au commit
    commitSet(monCommit, "tree", "mon_hash");
    commitSet(monCommit, "auteur", "John Doe");
    commitSet(monCommit, "date", "2023-04-10");
    commitSet(monCommit, "message", "Mon premier commit");

    // Récupération des informations du commit
    printf("Arbre : %s\n", commitGet(monCommit, "tree"));
    printf("Auteur : %s\n", commitGet(monCommit, "auteur"));
    printf("Date : %s\n", commitGet(monCommit, "date"));
    printf("Message : %s\n", commitGet(monCommit, "message"));
    
    // Conversion du commit en chaîne de caractères
    char* commitEnTexte = cts(monCommit);
    printf("Commit sous forme de texte :\n%s\n", commitEnTexte);

    // Conversion de la chaîne de caractères en commit
    Commit* commitDepuisTexte = stc(commitEnTexte);
    printf("Commit depuis le texte :\n%s\n", cts(commitDepuisTexte));

    // Sauvegarde du commit dans un fichier
    ctf(monCommit, "TestFiles/monCommit.txt");
    
    // Lecture d'un commit depuis un fichier
    Commit* commitDepuisFichier = ftc("TestFiles/monCommit.txt");
    
    printf("Commit depuis le fichier :\n%s\n", cts(commitDepuisFichier));
    
    // Création d'un blob pour le commit
    char* commitBlob = blobCommit(monCommit);
    
    printf("Le hash du commit blob est : %s\n", commitBlob);
    
    // Libération de la mémoire
    free(commitEnTexte);
    free(commitBlob);
    
    return 0;
}
