#include "Exercice9.h"


int main(int argc, char *argv[]) {
    
    // Test de restoreCommit
    // Remplacez "abc123" par un hash de commit réel de votre dépôt myGit.
    char *commit_hash = "abc123";
    printf("Restauration du commit %s\n", commit_hash);
    restoreCommit(commit_hash);
    
    // Test de myGitCheckoutBranch
    // Remplacez "test-branch" par le nom d'une branche réelle de votre dépôt myGit.
    char *branch_name = "test-branch";
    printf("Changement de branche vers %s\n", branch_name);
    myGitCheckoutBranch(branch_name);

    // Test de filterList
    List *L = getAllCommits();
    // Remplacez "ab" par un pattern correspondant à certains commits de votre dépôt myGit.
    char *pattern = "ab";
    printf("Filtrage des commits commençant par %s\n", pattern);
    List *list_trie = filterList(L, pattern);
    for (List ptr = *list_trie; ptr != NULL; ptr = ptr->next) {
        printf("-> %s\n", ptr->data);
    }

    // Test de myGitCheckoutCommit
    // Remplacez "ab" par un pattern correspondant à un commit unique de votre dépôt myGit.
    char *commit_pattern = "ab";
    printf("Changement vers le commit commençant par %s\n", commit_pattern);
    myGitCheckoutCommit(commit_pattern);

    // Libérer la mémoire
    freeList(L);
    freeList(list_trie);
    
    return 0;
}