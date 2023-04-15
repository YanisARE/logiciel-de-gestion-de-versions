#include "../headers/exercice4.h"



//Exercice4:

int getChmod ( const char * path ){
    struct stat ret;

    if ( stat (path , &ret) == -1) {
        return -1;
    }
    //remplacement des constantes par leur valeur a cause d'un bug 
    return (ret.st_mode & 0400)|( ret.st_mode & 0200 )|( ret.st_mode & 0100 )| /*owner*/
        (ret.st_mode & 0040 )|( ret.st_mode & 0020 )|( ret.st_mode & 0010 )|    /*group*/
        (ret.st_mode & 0004 )|( ret.st_mode & 0002 )|( ret.st_mode & 0001 );    /*other*/
}

void setMode(int mode, char* path) {
    char buff[100];
    sprintf(buff, "chmod %o %s", mode, path);
    system(buff);
}

// Q 4.1 -Permet de créer un workfile et de l'initialiser
WorkFile* createWorkFile(char* name){
    WorkFile * work= (WorkFile * ) malloc(sizeof(WorkFile));
    if (work==NULL || name==NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }
    work->name=strdup(name);
    work->hash=NULL;
    work->mode=getChmod(name);
    return work;
}

/* Q 4.2-permet de convertir un WorkFile en chaîne de caractères 
contenant les différents champs séparés par des tabulations
*/
char* wfts(WorkFile* wf) {
    //printf("Rentre dans wfts\n");
    int longnom = strlen(wf->name);
    //printf("Rentre apres premier strlen\n");
    int longhach ;
    if (wf->hash!=NULL){
        longhach = strlen(wf->hash);
    }
    //printf("Rentre apres deuxieme strlen\n");
    char* chaine = (char*)malloc(longnom + longhach + 12);
    //printf("Rentre apres allocution chaine et avant sprintf\n");
    sprintf(chaine, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, wf->mode);
    //printf("Rentre apres sprintf\n");
    return chaine;
}

// Q 4.3 -permet de convertir une chaîne de caractères représentant un WorkFile en un WorkFile
WorkFile* stwf(char* ch) {
    WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
    if (!wf) {
        printf("Erreur allocation de memoire\n");
        return NULL;
    }

    wf->name = (char *)malloc(300);
    wf->hash = (char *)malloc(300);
    sscanf(ch, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, &wf->mode);

    return wf;
}

/* Q 4.4-permet d'allouer un WorkTree de taille
fixée (donnée par une constante du programme) et de l'initialiser
*/
#define N 100
WorkTree* initWorkTree() {
    WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
    if (!wt) {
        printf("Erreur: lors allocution wt initWorkTree\n");
        return NULL;
    }

    wt->tab = (WorkFile *)malloc(N*sizeof(WorkFile));
    if (!(wt->tab)){
        printf("Erreur: allocution wt");
        return NULL;
    }
    wt->size = N;
    wt->n = 0;

    return wt;
}

/* Q 4.5-vérifie la présence d'un fichier ou répertoire dans un WorkTree. 
retourne la position du fichier dans le tableau s'il est présent, et -1 sinon.
*/
int inWorkTree(WorkTree* wt, char* nom) {
    if (wt == NULL || wt->tab == NULL || nom == NULL) {
        return -1;
    }

    for (int i = 0; i < wt->n; i++) {
        if (wt->tab[i].name != NULL && strcmp(wt->tab[i].name, nom) == 0) {
            return i;
        }
    }

    return -1;
}


// Q 4.6
/*int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    printf("Avant inWorkTree dans append worktree");
    printf("wt vaut : %p",wt);
    printf("name vaut : %s",name);
    if (inWorkTree(wt, name) == -1) {
        printf("inWorkTree renvoie:",inWorkTree(wt, name) );
        if (wt->n >= wt->size) {
            printf("Le WorkTree est plein dans appendWorkTree\n");
            return -1;
        }
        WorkFile * wf = createWorkFile(name);
        wf->hash = strdup(hash);
        wf->mode = mode;
        wt->tab[wt->n++] = *wf; 
        return wt->n - 1;
    }
    return -1;
}
*/

/*Q 4.6-ajoute un fichier ou répertoire au WorkTree
Version modifié pour eviter les segmentation fault 
*/
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    printf("Avant inWorkTree dans append worktree");
    printf("wt vaut : %p",wt);
    printf("name vaut : %s",name);
    if (wt == NULL || name == NULL) {
        printf("Error: wt or name pointer is null in appendWorkTree()\n");
        return -1;
    }
    if (inWorkTree(wt, name) == -1) {
        printf("inWorkTree renvoie:",inWorkTree(wt, name) );
        if (wt->n >= wt->size) {
            printf("Le WorkTree est plein dans appendWorkTree\n");
            return -1;
        }
        WorkFile * wf = createWorkFile(name);
        if (wf == NULL) {
            printf("Error: couldn't create WorkFile in appendWorkTree()\n");
            return -1;
        }
        wf->hash = strdup(hash);
        wf->mode = mode;
        wt->tab[wt->n++] = *wf; 
        return wt->n - 1;
    }
    return -1;
}


// Q 4.7
/* Version creant des segmentation fault car on a oublie encore une fois de verifier les acces memoires null
char* wtts(WorkTree* wt) {
    int bufsize = wt->n * 300;
    char *chaine = (char *)malloc(bufsize);
    if (!chaine) {
        printf("Erreur lors de l'allocation de memoire dans wtts\n");
        return NULL;
    }

    chaine[0] = '\0';
    for (int i = 0; i < wt->n; i++) {
        char *ligne = wfts(&wt->tab[i]);
        strcat(chaine, ligne);
        strcat(chaine, "\n");
        free(ligne);
    }
    return chaine;
}
*/
/*
Q 4.7-convertit un WorkTree en une chaîne de caractères composée des
 représentations des WorkFile séparées par un saut de ligne (caractère '\n')
*/
char* wtts(WorkTree* wt) {
    if (!wt) {
        printf("Erreur : pointeur NULL dans wtts\n");
        return NULL;
    }
    int bufsize = wt->n * 300;
    char *chaine = (char *)malloc(bufsize);
    if (!chaine) {
        printf("Erreur lors de l'allocation de memoire dans wtts\n");
        return NULL;
    }

    chaine[0] = '\0';
    for (int i = 0; i < wt->n; i++) {
        if (!wt->tab) {
            printf("Erreur : pointeur NULL dans wtts (tab)\n");
            free(chaine);
            return NULL;
        }
        char *ligne = wfts(&wt->tab[i]);
        if (!ligne) {
            printf("Erreur : pointeur NULL dans wtts (ligne)\n");
            free(chaine);
            return NULL;
        }
        strcat(chaine, ligne);
        strcat(chaine, "\n");
        free(ligne);
    }
    return chaine;
}


/* Q 4.8-convertit une chaîne de caractères représentant un ´ WorkTree en un WorkTree

*/
WorkTree * stwt ( char * ch){
    if (ch==NULL){
        printf("Erreur:ch est null ");
        return NULL;
    }
     // On définit une variable pour suivre la position
    size_t pos = 0; //size_t=unsigned long 
    int n_pos = 0;
    // Le séparateur qui sert à séparer la chaîne
    int sep = '\n' ;
    // Un pointeur pour trouver le séparateur
    char * ptr;
    // Allocation de mémoire pour le résultat
    char * res = malloc ( sizeof ( char ) *10000) ;
    if (res==NULL){
        printf("Erreur lors allocution\n");
        return NULL;
    }
    // Initialisation du WorkTree
    printf("Avant wt = initWorkTree ()\n");
    WorkTree * wt = initWorkTree ();

    // Boucle pour parcourir la chaîne
    while (pos < strlen (ch)){
        printf("rentre dans la boucle while avant ptr = strchr (ch + pos , sep )");
        // Recherche du séparateur dans la chaîne
        ptr = strchr (ch + pos , sep );
        printf("rentre dans la boucle while");
        // Si on n'a pas trouvé de séparateur alors c'est la fin de la chaîne
        if (ptr == NULL )
            n_pos = strlen (ch)+1;
        else {
            // Sinon, on calcule la position du séparateur
            n_pos = ptr - ch + 1;
        }
        // Copie de la séquence actuelle dans la variable 'res'
        memcpy ( res , ch+pos , n_pos - pos - 1);
        // Ajout de '\0' pour marquer la fin de la chaîne
        res [ n_pos - pos - 1] = '\0' ;
        // Mise à jour de la position
        pos = n_pos ;
        // Conversion de la séquence en WorkFile
        WorkFile * wf = stwf ( res );
        // On ajoute le workfile au worktree
        appendWorkTree (wt , wf ->name , wf ->hash , wf -> mode );
    }
    return wt;
}


/* Q 4.9 - écrit dans le fichier file lachaîne de caractères représentant un WorkTree
*/
int wttf(WorkTree* wt, char* fichier) {
    // On convertit le WorkTree en une chaîne de caractères
    char *chaine = wtts(wt);
    // Si la chaîne n'existe pas on retourne une erreur

    if (!chaine) {
        return -1;
    }

    char path[100];
    sprintf(path, "TestFiles/%s", fichier);

    FILE *f = fopen(path, "w");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier\n");
        free(chaine);
        return -1;
    }

    // On écrit la chaîne dans le fichier
    fprintf(f, "%s", chaine);
    fclose(f);
    free(chaine);

    return 0;
}



/* Q4.10-construit un WorkTree à partir d'un
fichier qui contient sa représentation en chaîne de caractères.
*/
WorkTree* ftwt(char* fichier) {
    // Ouvre le fichier
    char path[100];
    sprintf(path, "TestFiles/%s", fichier);

    FILE *f = fopen(path, "r");
    // Si l'ouverture échoue, retourne un pointeur nul
    if (!f) return NULL;

    // Détermine la taille du fichier en utilisant fseek :
    
    fseek(f, 0, SEEK_END); //déplace le pointeur de position à la fin du fichier
    long taille = ftell(f); //détermine la taille du fichier
    fseek(f, 0, SEEK_SET); // Retourne le pointeur au début du fichier

    char *chaine = (char *)malloc(taille + 1);
    if (!chaine) return NULL;

    // Lit le contenu du fichier dans la chaîne
    fread(chaine, 1, taille, f);
    chaine[taille] = '\0';

    fclose(f);

    // Convertit la chaîne en WorkTree
    WorkTree *wt = stwt(chaine);

    free(chaine);

    return wt;
}



