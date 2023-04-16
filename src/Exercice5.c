#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../headers/Exercice5.h"

//Exercice 5
//Q1
char* hashToFile(char* hash){
    struct stat st;
    char* ch2 = strdup(hash); 
    ch2[2] = '\0';
    // Crée un dossier avec les deux premiers caractères du hash si ce dossier n'existe pas
    if (stat(ch2, &st) == -1) { // Crée un dossier avec les deux premiers caractères du hash si ce dossier n'existe pas, avec comme permission 
        mkdir(ch2,0700); /*Creer un repertoire si le dossier portent le nom du hash n'existe pas, et donne à l'utilisateur (l'octal 2), la persmission 7 (rwx, donc lire, ecrire et executer), le premier chiffre correspondantant aux permissions spéciales et le  
                            troisième chiffre correspondant aux permissions du groupe et aux permissions pour les autres utilisateurs du système
                            */
    }
    // Retourne le chemin du fichier
    return hashToPath(hash);
}
/*
Q1-crée un fichier temporaire
représentant le WorkTree pour pouvoir ensuite créer l’enregistrement instantané du WorkTree (avec
l’extension ”.t”
*/
char* blobWorkTree(WorkTree* wt){
    char fname [100] = "/tmp/myfileXXXXXX"; 
    int fd = mkstemp(fname);
    wttf(wt, fname);
    char* hash = sha256file(fname); //On calcule le hash du fichier
    char* ch = hashToFile(hash); //crée un fichier basé sur le hash et retourne le chemin du fichier
    strcat(ch, ".t");
    cp(ch, fname);
    return hash;
}


//Concatene les deux chemin passe en parametre
char* concat_paths(char* path1, char* path2) {
    char * res = malloc(strlen(path1) + strlen(path2) + 1);  //On alloue l'espace mémoire correspondant à la taille des deux chemins plus un, pour le caractère "/" 
    if (res == NULL) {
        printf("Erreur lors de l'allocation [concat_paths]\n");
        return NULL; 
    }
    strcpy(res, path1);
    strcat(res, "/");
    strcat(res, path2);
    return res;
}

/*Q2-etant donné un WorkTree dont le chemin est donné en paramètre, 
crée un enregistrement instantané de tout son contenu (de manière récursive), puis de lui même
L’appel à saveWorkTree permet de conserver une sauvegarde de l’état de plusieurs fichiers à un instant
donné
*/
char* saveWorkTree(WorkTree* wt, char* path) {
    struct stat path_stat;
    // Parcours du WorkTree
    for (int i = 0; i < wt->n; i++) {
        //printf("Avant char* absPath = concat_paths(path, wt->tab[i].name);\n");
        char* absPath = concat_paths(path, wt->tab[i].name);
        // Si le chemin existe
        if (stat(absPath, &path_stat) == 0) { // vérifie si la fonction stat récupère bien les informations du fichier
            //printf("Apres stat(absPath, &path_stat) == 0\n");
            if (S_ISREG(path_stat.st_mode)) { // Vérifie si c'est un fichier
                //printf("Apres S_ISREG(path_stat.st_mode et avant blobfile)\n");
                blobFile(absPath);
                //printf("blobfile\n");
                wt->tab[i].hash = sha256file(absPath);
                //printf("apres sha256file\n");
                wt->tab[i].mode = getChmod(absPath);
                //printf("getChmod\n");
            }
        } else {
            // Initialisation d'un nouveau WorkTree
            WorkTree* wt2 = initWorkTree();
            //printf("apres initWorktree\n");

            // Obtention de la liste des éléments dans le répertoire absPath
            List* L = listdir(absPath);
            if(L==NULL) return NULL;

            // Parcours de la liste des éléments du répertoire
            for (List ptr = *(L); ptr != NULL; ptr = ptr->next) {
                //printf(" apres for loop\n");

                // Ignorer les fichiers et dossiers cachés commençant par un '.'
                if (ptr->data[0] == '.') continue;

                struct stat dir_stat;

                // Vérification de l'existence du fichier ou du dossier
                if (stat(absPath, &dir_stat) == 0) {
                    //printf("apres if stat(absPath, &dir_stat) == 0\n");

                    // Récupération des permissions du répertoire
                    mode_t dir_mode = dir_stat.st_mode & (0x1C0 | 0x38 | 0x07);

                    // Ajout de l'élément au WorkTree wt2
                    appendWorkTree(wt2, ptr->data, 0, dir_mode);
                    //printf("apres appendWorktree\n");
                }
            }
            // Sauvegarde du WorkTree wt2 de manière récursive
            wt->tab[i].hash = saveWorkTree(wt2, absPath);

            // Récupération des permissions du fichier ou du dossier absPath
            wt->tab[i].mode = getChmod(absPath);
        }

    }
    //printf("avant blobWorkTree(wt)\n");
    return blobWorkTree(wt);
}


//QAnnexe- Permet de verifier l'existence du worktree
int isWorkTree(char* hash){
    if (file_exists(strcat(hashToPath(hash), ".t"))){
        return 1; 
    }
    if (file_exists(hashToPath(hash))){ 
        return 0;
    }
    return -1;
}
/*Q3-restaurer un WorkTree c’est-à-dire qui recrée l’arborescence des
fichiers comme décrit par ses enregistrements instantanés
*/
void restoreWorkTree(WorkTree* wt, char* path){
    // Parcours du WorkTree
    for(int i=0; i < wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name); 
        char* copyPath = hashToPath(wt->tab[i].hash);
        char* hash = wt->tab[i].hash;
        // Si le hash correspond à un fichier
        if (isWorkTree(hash) == 0){
            // Copie le fichier dans le chemin absolu
            cp(absPath , copyPath);
            // Modifie les permissions du fichier copié avec celles du fichier donne par le chemin copypath
            setMode(getChmod(copyPath),absPath); 
        } else {
            // Si le hash correspond à un WorkTree
            if(isWorkTree(hash) == 1){
                // Ajoute l'extension ".t" au chemin
                strcat(copyPath, ".t");
                // Crée un nouveau WorkTree à partir du fichier
                WorkTree* nwt = ftwt(copyPath); 
                // Restaure le WorkTree de manière récursive
                restoreWorkTree(nwt, absPath); 
                // Modifie les permissions du dossier copié
                setMode(getChmod(copyPath),absPath);
            }
        }
    }
}

