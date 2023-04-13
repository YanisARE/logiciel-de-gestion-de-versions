#include "../headers/myGit.h"


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