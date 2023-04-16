#include "../headers/Exercice9.h"


/*Q9.1
Permet de restaurer le worktree associé à un commit dont le hash est donné en paramètre.
*/
// Fonction pour restaurer un commit à partir de son hash
void restoreCommit(char* hash_commit) {
    char * path = malloc(6 + strlen(hash_commit));
    strcat(path, ".refs/");
    strcat(path, hash_commit);
    // On charge le commit à partir du chemin .refs/branch
    Commit* c = ftc(path);
    
    // On récupère le hash de l'arbre (tree) associé au commit
    // On convertit ce hash en chemin vers le fichier d'arbre correspondant avec hashToPath
    char* tree_hash = malloc(256);
    if(hashToPath(commitGet(c, "tree"))!=NULL){
        printf("Il est null hashTopath\n");
    
        strcat(tree_hash, hashToPath(commitGet(c, "tree")));
        strcat(tree_hash, ".t");

        // On stock le worktree cree à partir  du chemin vers le fichier tree_hash
        WorkTree* wt = ftwt(tree_hash);

        // on restaure l'arbre de travail dans le répertoire courant
        restoreWorkTree(wt, ".");
    }
}


/*Q9.2
La fonction myGitCheckoutBranch permet de changer la branche courante en spécifiant le nom de la nouvelle branche en paramètre, 
et de restaurer le worktree associé au dernier commit de cette branche.
*/
void myGitCheckoutBranch(char* branch) {
    FILE* f = fopen(".current_branch", "w");
    fprintf(f, "%s", branch);
    fclose(f);

    // Récupération du hash du dernier commit de la branche courante
    char* hash_commit = getRef(branch);
    //On modifie la référence HEAD pour pointer vers le dernier commit de la branche courante
    createUpdateRef("HEAD", hash_commit);
    // Restauration du worktree correspondant au dernier commit de la branche courante
    restoreCommit(branch);
}


/*
Q9.3:
 retourne une nouvelle liste contenant uniquement les éléments de L qui commencent par la chaîne de caractères pattern.
*/
List *filterList(List *L, char *pattern) {
    //On initialise une nouvelle liste pour stocker les element qui commencent par le pattern
    List *filtre = initList();
    //On parcourt la liste L
    for (Cell *ptr = *L; ptr != NULL; ptr = ptr->next) {
        // Duplication de l'élément courant de la liste L pour éviter de modifier le contenu de la liste
        char *c = strdup(ptr->data);
        // Mise à zéro du caractère après le dernier caractère du pattern
        c[strlen(pattern)] = '\0';
        if (strcmp(c, pattern) == 0) {
            // Insertion de l'élément courant dans la nouvelle liste filtrée
            insertFirst(filtre, buildCell(ptr->data));
        }
        free(c);
    }
    return filtre;
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

/*Q9.4
  void myGitCheckoutCommit(char* pattern) :
    -modifie le fichier .current branch pour contenir le nom de la branche donné en paramètre.
    —modifie la référence HEAD pour contenir le hash du dernier commit de branch (on rappelle
    que ce hash est contenu dans le fichier branch).
    —Restaure le worktree correspondant au dernier commit de branch
 */
void myGitCheckoutCommit(char *pattern) {
    // On récupère la liste de tous les commits
    List *L = getAllCommits();
    // On filtre la liste pour ne conserver que les commits qui commencent par le motif pattern
    List *filtred_list = filterList(L, pattern);
    // Si le filtrage ne laisse qu'un seul commit
    if (listSize(filtred_list) == 1) {
        // On récupère le hash de ce commit
        char *commit_hash = (listGet(filtred_list, 0))->data;
        // On modifie la référence HEAD pour pointer sur ce commit
        createUpdateRef("HEAD", commit_hash);
        // On restaure le worktree correspondant au dernier commit de la branche
        restoreCommit(commit_hash);
    } else {
        // Si le filtrage ne donne aucun résultat
        if (listSize(filtred_list) == 0) {
            printf("Aucun commit trouvé.\n");
        } else {
            // Si le filtrage donne plusieurs résultats
            printf("Plusieurs commit trouvé:\n");
            // On affiche tous les commits trouvés
            for (Cell *ptr = *filtred_list; ptr != NULL; ptr = ptr->next) {
                printf("-> %s\n", ptr->data);
            }
        }
    }
}