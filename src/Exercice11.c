#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/Exercice11.h"

//Questin 1
void addToList(List** list, char* value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;

    if ((*list)->head == NULL) {
        (*list)->head = node;
    } else {
        Node* current = (*list)->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    (*list)->size++;
}

int isInConflict(char* name, WorkTree* wt1, WorkTree* wt2) {
    int i;
    for (i = 0; i < wt1->n; i++) {
        if (strcmp(wt1->tab[i].name, name) == 0) {
            int j;
            for (j = 0; j < wt2->n; j++) {
                if (strcmp(wt2->tab[j].name, name) == 0) {
                    if (strcmp(wt1->tab[i].hash, wt2->tab[j].hash) != 0 ||
                        wt1->tab[i].mode != wt2->tab[j].mode) {
                        return 1;
                    }
                }
            }
            return 0;
        }
    }
    for (i = 0; i < wt2->n; i++) {
        if (strcmp(wt2->tab[i].name, name) == 0) {
            return 0;
        }
    }
    return -1;
}

WorkTree* mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts) {
    WorkTree* merged = (WorkTree*) malloc(sizeof(WorkTree));
    merged->size = wt1->size + wt2->size;
    merged->n = 0;
    merged->tab = (WorkFile*) malloc(merged->size * sizeof(WorkFile));

    //Parcourir tous les fichiers de wt1
    for (int i = 0; i < wt1->n; i++) {
        WorkFile* wf = &(wt1->tab[i]);

        //Trouver un fichier correspondant dans wt2
        WorkFile* wf2 = findFile(wt2, wf->name);

        if (wf2 == NULL) {
            //Si le fichier n'est pas dans wt2, on l'ajoute dans merged tree
            merged->tab[merged->n++] = *wf;
        } else if (strcmp(wf->hash, wf2->hash) != 0) {
            //Si le fichier est dans les deux Trees, Mais les hash sont différents - alors ça crée un conflict
            addToList(conflicts, wf->name);
        }
    }

    //Parcourir tous les fichiers du wt2
    for (int i = 0; i < wt2->n; i++) {
        WorkFile* wf = &(wt2->tab[i]);

        //Vérifier si tous les fichiers sont dans merged tree
        WorkFile* wf_merged = searchList(merged, wf->name);

        if (wf_merged == NULL) {
            // Si fichier n'est pas dans merged tree, on l'ajoute
            merged->tab[merged->n++] = *wf;
        }
    }

    return merged;
}

//Question 2
List* merge(char* remote_branch, char* message) {
    // Vérification préalable : la branche courante est-elle en cours de modification ?
    if (is_working_tree_modified()) {
        printf("Impossible de fusionner : la branche courante est en cours de modification.\n");
        return NULL;
    }

    // Vérification préalable : la branche distante existe-t-elle ?
    Commit* remote_head = get_branch_head(remote_branch);
    if (remote_head == NULL) {
        printf("Impossible de fusionner : la branche distante %s n'existe pas.\n", remote_branch);
        return NULL;
    }

    // Fusion des worktrees
    List* conflicts = NULL;
    WorkTree* merged_tree = mergeWorkTrees(get_working_tree(), get_work_tree_of_commit(remote_head), &conflicts);

    // Vérification : y a-t-il des conflits ?
    if (conflicts != NULL) {
        printf("Impossible de fusionner : des conflits ont été détectés.\n");
        return conflicts;
    }

    // Création du nouveau commit
    char* new_commit_hash = create_commit(merged_tree, get_head_commit(), remote_head, message);

    // Enregistrement instantané du worktree de fusion et du nouveau commit
    blobCommit(new_commit_hash);

    // Ajout du nouveau commit à la branche courante
    add_commit_to_current_branch(new_commit_hash);

    // Mise à jour de la référence HEAD
    set_head_commit(new_commit_hash);

    // Suppression de la branche distante
    deleteRef(remote_branch);

    // Restauration du projet correspondant au worktree de fusion
    restore_working_tree(merged_tree);

    return NULL;
}
