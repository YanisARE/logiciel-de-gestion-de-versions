#include "../headers/myGit.h"
#include "../headers/Exercice9.h"

/*
int main ( int argc , char * argv []) {
    if ( strcmp ( argv [1] , " init ") == 0) {
        initRefs ();
    }
    if ( strcmp ( argv [1] , " list-refs ") == 0) {
        printf ("REFS : \n");
        if ( file_exists (".refs")){
                List * L = listdir (".refs");
            for ( Cell * ptr = *L; ptr != NULL ; ptr = ptr -> next ) {
                if (ptr -> data [0] == '.' )
                    continue ;
                char * content = getRef (ptr -> data );
                printf ("− %s \t %s \n", ptr ->data , content );
            }
        }
    }
    if ( strcmp ( argv [1] , "create-ref") == 0) {
        createUpdateRef ( argv [2] , argv [3]) ;
    }
    if ( strcmp ( argv [1] , "delete-ref") == 0) {
        delete ( argv [2]) ;
    }
    if ( strcmp ( argv [1] , "add") == 0) {
        for (int i = 2; i < argc ; i++) {
            myGitAdd ( argv [i]);
        }
    }
    if ( strcmp ( argv [1] , " clear-add") == 0) {
        system ("rm . add");
    }
    if ( strcmp ( argv [2] ,"list-add ") ==0) {
        printf ("Zone de preparation : \n");
        if ( file_exists (" .add")){
            WorkTree * wt = ftwt (" .add");
            printf ("%s \n", wtts (wt));
        }
    }
    if ( strcmp ( argv [1] , "commit") == 0) {
        if( strcmp ( argv [3] ,"−m")== 0){
            myGitCommit ( argv [2] , argv [4]) ;
        } else {
            myGitCommit ( argv [2] , NULL );
        }
    }
    return 0;
}
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilisation : %s commande [args...]\n", argv[0]);
        printf("Commandes disponibles :\n");
        printf("  init : initialiser le fichier de références\n");
        printf("  list-refs : lister toutes les références et leur hachage correspondant\n");
        printf("  create-ref <nom> <hachage> : créer ou mettre à jour une référence avec le nom et le hachage donnés\n");
        printf("  delete-ref <nom> : supprimer la référence avec le nom donné\n");
        printf("  add <fichier_ou_dossier> [fichier_ou_dossier]... : ajouter le(s) fichier(s) ou dossier(s) donné(s) à l'index\n");
        printf("  clear-add : vider l'index la zone de preparation\n");
        printf("  list-add : affiche le contenu de la zone de preparation\n");
        printf("  commit <branche> [-m <message>] : commiter les modifications de l'index à la branche donnée avec un message optionnel\n");
        // Ajout des commandes pour l'exercice 10
        printf("  get-current-branch : affiche la branche courante\n");
        printf("  branch <nom> : créer une nouvelle branche avec le nom donné\n");
        printf("  branch-print <nom> : affiche les commits de la branche donnée\n");
        printf("  checkout-branch <nom> : changer de branche et restaurer le worktree associé\n");
        printf("  checkout-commit <pattern> : changer de commit et restaurer le worktree associé\n");
    return -1;
    }


    if (strcmp(argv[1], "init") == 0) {
        initRefs();
        initBranch();
    }

    if (strcmp(argv[1], "list-refs") == 0) {
        printf("REFS : \n");
        if (file_exists(".refs")) {
            List *L = listdir(".refs");
            if (L) {
                for (Cell *ptr = *L; ptr != NULL; ptr = ptr->next) {
                    if (ptr->data[0] == '.')
                        continue;
                    char *content = getRef(ptr->data);
                    if (content) {
                        printf("− %s \t %s \n", ptr->data, content);
                        free(content);
                    }
                }
                freeList(L);
            }
        }
    }

    if (strcmp(argv[1], "create-ref") == 0) {
        if (argc != 4) {
            printf("Usage: ./program create-ref <ref_name> <hash>\n");
            return -1;
        }
        createUpdateRef(argv[2], argv[3]);
    }

    if (strcmp(argv[1], "delete-ref") == 0) {
        if (argc != 3) {
            printf("Usage: ./program delete-ref <ref_name>\n");
            return -1;
        }
        deleteRef(argv[2]);
    }

    if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("Usage: ./program add <file_or_folder1> <file_or_folder2> ...\n");
            return -1;
        }
        for (int i = 2; i < argc; i++) {
            myGitAdd(argv[i]);
        }
    }

    if (strcmp(argv[1], "clear-add") == 0) {
        system("rm .add");
    }

    if (strcmp(argv[1], "list-add") == 0) {
        printf("Zone de preparation : \n");
        if (file_exists(".add")) {
            WorkTree *wt = ftwt(".add");
            if (wt) {
                printf("%s \n", wtts(wt));
                freeWorkTree(wt);
            }
        }
    }

    if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3 || argc > 5) {
            printf("Usage: ./program commit <branch_name> [-m <message>]\n");
            return -1;
        }
        char *message = NULL;
        if (argc == 5 && strcmp(argv[3], "-m") == 0) {
            message = argv[4];
        }
        myGitCommit(argv[2], message);
    }
    //Ajout de l'exercice 10 :
    if (strcmp(argv[1], "get-current-branch") == 0) {
        printf("%s\n", getCurrentBranch());
    }

    if (strcmp(argv[1], "branch") == 0) {
        if (!branchExists(argv[2])) {
            createBranch(argv[2]);
        } else {
            printf("La branche exisre déjà.\n");
        }
    }

    if (strcmp(argv[1], "branch-print") == 0) {
        if (!branchExists(argv[2])) {
            printf("La branche n'existe pas.\n");
        } else {
            printBranch(argv[2]);
        }
    }

    if (strcmp(argv[1], "checkout-branch") == 0) {
        if (!branchExists(argv[2])) {
            printf("La branche n'existe pas.\n");
        } else {
            myGitCheckoutBranch(argv[2]);
        }
    }

    if (strcmp(argv[1], "checkout-commit") == 0) {
        myGitCheckoutCommit(argv[2]);
    }
    return 0;
}
