#include "../headers/exercice4.h"


#define EX4_MAIN

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

// Q 4.1
WorkFile* createWorkFile(char* name){
    WorkFile * work= (WorkFile * ) malloc(sizeof(WorkFile));
    if (!work) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }
    work->name=strdup(name);
    work->hash=NULL;
    work->mode=getChmod(name);
    return work;
}

// Q 4.2
char* wfts(WorkFile* wf) {
    int longnom = strlen(wf->name);
    int longhach = strlen(wf->hash);
    char* chaine = (char*)malloc(longnom + longhach + 12);

    sprintf(chaine, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, wf->mode);

    return chaine;
}

// Q 4.3
WorkFile* stwf(char* ch) {
    WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
    if (!wf) {
        printf("Erreur d'allocation de memoire\n");
        return NULL;
    }

    wf->name = (char *)malloc(300);
    wf->hash = (char *)malloc(300);
    sscanf(ch, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, &wf->mode);

    return wf;
}

// Q 4.4
#define N 100
WorkTree* initWorkTree() {
    WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
    if (!wt) {
        printf("Erreur d'allocation de mémoire dans initWorkTree\n");
        return NULL;
    }

    wt->tab = (WorkFile *)malloc(N*sizeof(WorkFile));
    wt->size = N;
    wt->n = 0;

    return wt;
}

// Q 4.5
int inWorkTree(WorkTree* wt, char* nom) {
    for (int i = 0; i < wt->n; i++) {
        if (strcmp(wt->tab[i].name, nom) == 0) {
            return i;
        }
    }

    return -1;
}

// Q 4.6
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    if (inWorkTree(wt, name) == -1) {
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

// Q 4.7
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



// Q 4.8
WorkTree * stwt ( char * ch){
     // On définit une variable pour suivre la position
    size_t pos = 0;
    int n_pos = 0;
    // Le séparateur qui sert à séparer la chaîne
    int sep = '\n' ;
    // Un pointeur pour trouver le séparateur
    char * ptr;
        // Allocation de mémoire pour le résultat
    char * res = malloc ( sizeof ( char ) *10000) ;
    // Initialisation du WorkTree
    WorkTree * wt = initWorkTree ();

    // Boucle pour parcourir la chaîne
    while (pos < strlen (ch)){
        // Recherche du séparateur dans la chaîne
        ptr = strchr (ch + pos , sep );
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


// Q 4.9
int wttf(WorkTree* wt, char* fichier) {
    // On convertit le WorkTree en une chaîne de caractères
    char *chaine = wtts(wt);
    // Si la chaîne n'existe pas on retourne une erreur

    if (!chaine) {
        return -1;
    }

    FILE *f = fopen(fichier, "w");
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


// Q 4.10
WorkTree* ftwt(char* fichier) {
    // Ouvre le fichier
    FILE *f = fopen(fichier, "r");
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


