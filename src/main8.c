#include "../headers/Exercice8.h"



int main() {
    // Initialise une branche
    initBranch();

    // Vérifie si une branche existe
    char* branch_name = "new_branch";
    
    if (branchExists(branch_name)) {
        printf("La branche %s existe.\n", branch_name);
    } else {
        printf("La branche %s n'existe pas.\n", branch_name);
    }
    /*
    */
    /*
    // Crée une nouvelle branche
    createBranch(branch_name);

    // Vérifie à nouveau si la branche existe
    if (branchExists(branch_name)) {
        printf("La branche %s existe.\n", branch_name);
    } else {
        printf("La branche %s n'existe pas.\n", branch_name);
    }

    // Récupère et affiche la branche actuelle
    char* current_branch = getCurrentBranch();
    printf("La branche actuelle est : %s\n", current_branch);

    // Affiche les commits d'une branche
    printBranch(current_branch);

    // Récupère la liste des commits pour une branche
    List* commit_list = branchList(current_branch);

    // Récupère la liste de tous les commits pour toutes les branches
    List* all_commits = getAllCommits();

    // Libère la mémoire allouée
    
    free(current_branch);
    */
    return 0;
}