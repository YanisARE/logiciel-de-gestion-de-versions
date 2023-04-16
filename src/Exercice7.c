#include "../headers/Exercice7.h"

/*
    7.1-crée le répertoire caché .refs
*/
void initRefs(){
    if (! file_exists (".refs")){ //On test si il n'existe pas, si oui alors on les crée 
        system ("mkdir .refs ");
        system (" touch .refs/master ");
        system (" touch .refs/HEAD");
    }
}

 /*7.2-met à jour une référence en remplaçant son contenu par hash. 
 Si la référence n'existe pas, la fonction commence par créer le fichier.*/
void createUpdateRef(char* ref , char* hash){
    initRefs(); //On crée la référence si elle n'existe pas 
    char buffer [256];
    sprintf (buffer , " echo %s > .refs/%s ", hash , ref );
    system ( buffer );
}

/*Q.Annexe-permet de supprimer une référence*/
void deleteRef ( char * ref ){
    char buffer [256];
    sprintf (buffer , " .refs/%s ", ref );
    if (! file_exists ( buffer )){
        printf ("Le fichier %s n'existe pas\n", ref );
    }else{
        sprintf(buffer,"rm .refs/%s ",ref);
        system(buffer);
    }
}

/*7.4-Récupère vers quoi pointe une référence (c-à-d le hash contenu dans le fichier). 
    Si le fichier est vide, la fonction retourne une chaîne de caractère vide. Si le fichier n’existe pas, la fonciton retourne NULL.*/
char * getRef ( char * ref_name ){
    
    FILE *f;
    char * res = malloc ( sizeof ( char ) *256) ;
    char buff [256];
    sprintf (buff ,  " .refs/%s ", ref_name );
    if (! file_exists ( buff )){
        printf ("Le fichier %s n'existe pas\n ", ref_name );
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

/*Q.Annexe-Permet de créer un fichier */
void createFile ( char * file ){
    if (file==NULL) return;
    char buff [256];
    sprintf (buff , " touch %s ", file );
    system ( buff );
}

/*7.5-permet à un utilisateur d’ajouter un fichier ou un répertoire dans le WorkTree correspondant à la zone de préparation. Si le
    fichier ".add" n’existe pas, il faudra d’abord le créer.*/
void myGitAdd (char * File_or_Folder ){
    WorkTree * wt;
    if (! file_exists (".add")){
        createFile (".add");
        wt = initWorkTree ();
    } else {
        wt = ftwt (".add");
    }
    if ( file_exists ( File_or_Folder )){
        appendWorkTree (wt , File_or_Folder , "NULL",  0644); //NULL ne marche pas remplacé par 0644
        wttf (wt , ".add");
    } else {
        printf (" Le fichier ou repertoire %s n'existe pas \n", File_or_Folder );
        return;
    }
}

// Vérifie si le fichier .refs existe
/* Meme probleme acces memoire interdit
void myGitCommit(char *nom_branche, char *message) {
    if (!file_exists(".refs")) {
        printf("Il faut initialiser le fichier ref");
        return;
    }
    
    // Vérifie si la branche existe
    char chemin_branche[256];
    sprintf(chemin_branche, ".refs/%s", nom_branche);
    if (!file_exists(chemin_branche)) {
        printf("La branche n'existe pas");
        return;
    }
    
    // Récupère les hashs des derniers commits sur la branche et sur HEAD
    char *dernier_hash = getRef(nom_branche);
    char *dernier_head_hash = getRef("HEAD");
    
    // Vérifie si le hash du dernier commit sur la branche est le même que celui sur HEAD
    if (strcmp(dernier_hash, dernier_head_hash) != 0) {
        printf("Erreur: HEAD est différent du dernier commit");
        return;
    }
    
    // Crée un nouveau commit avec les modifications apportées au Working Tree
    WorkTree *wt = ftwt(".add");
    char *hash_wt = saveWorkTree(wt, ".");
    Commit *c = createCommit(hash_wt, 0);
    
    // Définit le hash du dernier commit comme prédecesseur du nouveau commit, s'il existe
    if (strlen(dernier_hash) != 0) {
        commitSet(c, "predecesseur", dernier_hash);
    }
    
    // Ajoute un message au nouveau commit, s'il est fourni
    if (message != NULL) {
        commitSet(c, "message", message);
    }
    
    // Enregistre le nouveau commit et met à jour la référence de la branche et de HEAD
    char *hash_commit = blobCommit(c);
    createUpdateRef(nom_branche, hash_commit);
    createUpdateRef("HEAD", hash_commit);
    
    // Supprime les fichiers ajoutés à l'index de Git
    system("rm .add");
}
*/
//Q.Annexe-Fonction annexe a myGitCommit qui free un worktree : 
void freeWorkTree(WorkTree* wt) {
    if (wt == NULL) {
        return;
    }

    for (int i = 0; i < wt->n; i++) {
        free(wt->tab[i].name);
        free(wt->tab[i].hash);
    }
    free(wt->tab);
    free(wt);
}

/*
7.6-réalise un commit, c’est-à-dire créer un point de sauvegarde à la
manière de la commande git commit
*/
void myGitCommit(char *nom_branche, char *message) {
    if (!file_exists(".refs")) {
        printf("Il faut initialiser le fichier ref\n");
        return;
    }

    // Vérifie si la branche existe
    char chemin_branche[256];
    sprintf(chemin_branche, ".refs/%s", nom_branche);
    if (!file_exists(chemin_branche)) {
        printf("La branche n'existe pas\n");
        return;
    }

    // Récupère les hashs des derniers commits sur la branche et sur HEAD
    char *dernier_hash = getRef(nom_branche);
    char *dernier_head_hash = getRef("HEAD");

    // Vérifie si le hash du dernier commit sur la branche est le même que celui sur HEAD
    if (dernier_hash == NULL || dernier_head_hash == NULL) {
        printf("Erreur: dernier_hash ou dernier_head_hash est NULL\n");
        return;
    }
    if (strcmp(dernier_hash, dernier_head_hash) != 0) {
        printf("Erreur: HEAD est différent du dernier commit\n");
        return;
    }

    // Crée un nouveau commit avec les modifications apportées au Working Tree
    WorkTree *wt = ftwt(".add");
    if (wt == NULL) {
        printf("Erreur: wt est NULL\n");
        return;
    }
    char *hash_wt = saveWorkTree(wt, ".");
    if (hash_wt == NULL) {
        printf("Erreur: hash_wt est NULL\n");
        freeWorkTree(wt);
        return;
    }
    Commit *c = createCommit(hash_wt, 0);
    if (c == NULL) {
        printf("Erreur: c est NULL\n");
        freeWorkTree(wt);
        free(hash_wt);
        return;
    }

    // Définit le hash du dernier commit comme prédecesseur du nouveau commit, s'il existe
    if (strlen(dernier_hash) != 0) {
        commitSet(c, "predecesseur", dernier_hash);
    }

    // Ajoute un message au nouveau commit, s'il est fourni
    if (message != NULL) {
        commitSet(c, "message", message);
    }

    // Enregistre le nouveau commit et met à jour la référence de la branche et de HEAD
    char *hash_commit = blobCommit(c);
    if (hash_commit == NULL) {
        printf("Erreur: hash_commit est NULL\n");
        freeWorkTree(wt);
        free(hash_wt);
        free(c);
        return;
    }
    createUpdateRef(nom_branche, hash_commit);
    createUpdateRef("HEAD", hash_commit);

    // Supprime les fichiers ajoutés à l'index de Git
    system("rm .add");
}



