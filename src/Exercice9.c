#include "../headers/Exercice9.h"


/*Q1
Permet de restaurer le worktree associé à un commit dont le hash est donné en paramètre.
*/
void restoreCommit(char* hash_commit) {
    char* chemin = hashToPathCommit(hash_commit);
    Commit* c = ftc(chemin);
    char* tree_hash = strcat(hashToPath(commitGet(c, "tree")), ".t");
    WorkTree* wt = ftwt(tree_hash);
    restoreWorkTree(wt, ".");
}

/*Q2
La fonction myGitCheckoutBranch permet de changer la branche courante en spécifiant le nom de la nouvelle branche en paramètre, 
et de restaurer le worktree associé au dernier commit de cette branche.
*/
void myGitCheckoutBranch(char* branch) {
    FILE* f = fopen(".current_branch", "w");
    fprintf(f, "%s", branch);
    fclose(f);

    char* hash_commit = getRef(branch);
    createUpdateRef("HEAD", hash_commit);
    restoreCommit(hash_commit);
}


/*
Q3:
 retourne une nouvelle liste contenant uniquement les éléments de L qui commencent par la chaîne de caractères pattern.
*/
List *filterList(List *L, char *pattern) {
    List *filtered = initList();
    for (Cell *ptr = *L; ptr != NULL; ptr = ptr->next) {
        char *c = strdup(ptr->data);
        c[strlen(pattern)] = '\0';
        if (strcmp(c, pattern) == 0) {
            insertFirst(filtered, buildCell(ptr->data));
        }
        free(c);
    }
    return filtered;
}
//Fonction annexe de myGitCheckoutCommit :
/*
prend un pointeur vers une List en argument et renvoie le nombre d'éléments dans la liste. 
La fonction parcourt la liste en utilisant un pointeur current et incrémente un compteur count jusqu'à ce qu'elle atteigne la fin de la liste (quand current est égal à NULL)
*/
int listSize(List *L) {
    int count = 0;
    Cell *current = *L;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

/*Q4
  void myGitCheckoutCommit(char* pattern) qui procède comme décrit au début de l'exercice.
 */
void myGitCheckoutCommit(char *pattern) {
    List *L = getAllCommits();
    List *filtred_list = filterList(L, pattern);
    if (listSize(filtred_list) == 1) {
        char *commit_hash = (listGet(filtred_list, 0))->data;
        createUpdateRef("HEAD", commit_hash);
        restoreCommit(commit_hash);
    } else {
        if (listSize(filtred_list) == 0) {
            printf("No pattern matching.\n");
        } else {
            printf("Multiple matchings found:\n");
            for (Cell *ptr = *filtred_list; ptr != NULL; ptr = ptr->next) {
                printf("-> %s\n", ptr->data);
            }
        }
    }
}