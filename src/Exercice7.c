#include "Exercice7.h"


void initRefs(){
    /*
    crée le répertoire caché .refs
    */
    if (! file_exists (".refs")){ //On test si il n'existe pas, si oui alors on les crée 
        system ("mkdir .refs ");
        system (" touch .refs/master ");
        system (" touch .refs/HEAD");
    }
}

void createUpdateRef(char* ref , char* hash){
    /*met à jour
    une référence en remplaçant son contenu par hash. Si la référence n'existe pas, la fonction commence par créer le fichier.*/
    initRefs(); //On crée la référence si elle n'existe pas 
    char buffer [256];
    sprintf (buffer , " echo %s > .refs/%s ", hash , ref );
    system ( buffer );
}

void deleteRef ( char * ref ){
    /*permet de supprimer une référence*/
    char buffer [256];
    sprintf (buffer , " .refs/%s ", ref );
    if (! file_exists ( buffer )){
        printf ("Le fichier %s n'existe pas ", ref );
    }else{
        sprintf(buffer,"rm .ref/%s ",ref);
    }
}

char * getRef ( char * ref_name ){
    /*Récupère vers quoi pointe une référence (c-à-d le hash contenu dans le fichier). 
    Si le fichier est vide, la fonction retourne une chaîne de caractère vide. Si le fichier n’existe pas, la fonciton retourne NULL.*/
    FILE *f;
    char * res = malloc ( sizeof ( char ) *256) ;
    char buff [256];
    sprintf (buff ,  " .refs/%s ", ref_name );
    if (! file_exists ( buff )){
        printf ("Le fichier %s n'existe pas ", ref_name );
    return NULL ;
    }
    f = fopen (buff , "r");
    if (f == NULL ) {
        printf ("Erreur lors de l'ouverture du fichier\n");
        return NULL ;
    }
    fgets (res , 256 , f);
    fclose (f);
    return res ;
}

void createFile ( char * file ){
    /*Permet de créer un fichier */
    char buff [256];
    sprintf (buff , " touch %s ", file );
    system ( buff );
}
void myGitAdd ( char * File_or_Folder ){
    /*permet à un utilisateur d’ajouter un fichier ou un répertoire dans le WorkTree correspondant à la zone de préparation. Si le
    fichier ".add" n’existe pas, il faudra d’abord le créer.*/
    WorkTree * wt;
    if (! file_exists (".add")){
        createFile (".add");
        wt = initWorkTree ();
    } else {
        wt = ftwt (".add");
    }
    if ( file_exists ( File_or_Folder )){
        appendWorkTree (wt , File_or_Folder , 0, NULL );
        wttf (wt , ".add");
    } else {
        printf (" Le fichier ou repertoire %s n'existe pas \n", File_or_Folder );
    }
}

void myGitCommit ( char * branch_name , char * message ){
    if (! file_exists (".refs")){
        printf ("Il faut initialiser le fichier ref");
    }
    return ;
}

