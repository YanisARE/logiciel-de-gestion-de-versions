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
    // Calcule la longueur du nom du fichier
    int longnom = strlen(wf->name);
    
    //printf("Rentre apres premier strlen\n");
    // Calcule la longueur du hash du fichier s'il existe
    int longhach ;
    if (wf->hash!=NULL){
        longhach = strlen(wf->hash);
    }

    //printf("Rentre apres deuxieme strlen\n");
    // Alloue de l'espace pour la chaîne de caractères qui représente le WorkFile
    char* chaine = (char*)malloc(longnom + longhach + 12);


    //printf("Rentre apres allocution chaine et avant sprintf\n");
    // Crée la chaîne de caractères représentant le WorkFile sous le format "nom:<nom_fichier>\thash:<hash_fichier>\tmode:<mode_fichier>"
    sprintf(chaine, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, wf->mode);

    //printf("Rentre apres sprintf\n");
    return chaine;
}

// Q 4.3 -permet de convertir une chaîne de caractères représentant un WorkFile en un WorkFile
WorkFile* stwf(char* ch) {

    // Alloue de l'espace mémoire pour un WorkFile
    WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
    if (!wf) {
        printf("Erreur allocation de memoire\n");
        return NULL;
    }

    wf->name = (char *)malloc(300); //on alloue l'espace memoire necessaire pour le nom et le hash  
    wf->hash = (char *)malloc(300);

    //Remplie le workfile à partir de la chaine de caractère 
    sscanf(ch, "nom:%s\thash:%s\tmode:%d", wf->name, wf->hash, &wf->mode);

    return wf;
}

/* Q 4.4-permet d'allouer un WorkTree de taille
fixée (donnée par une constante du programme) et de l'initialiser
*/
#define N 100
WorkTree* initWorkTree() {
    // Alloue de l'espace mémoire pour un WorkTree
    WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
    if (!wt) {
        printf("Erreur: lors allocution wt initWorkTree\n");
        return NULL;
    }

    // Alloue de l'espace mémoire pour le tableau de WorkFile
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
        if (wt->tab[i].name != NULL && strcmp(wt->tab[i].name, nom) == 0) { //On parcourt le worktree en vérifiant qu'il ne soit pas null et si le nom du workfile courant est égale au nom passe en paramètre 
            return i;
        }
    }

    return -1;
}

//Q 4.6-ajoute un fichier ou répertoire au WorkTree
int appendWorkTree(WorkTree* wt, char* name, char* hash, int mode) {
    // Vérifie si les pointeurs sont non null
    if (wt == NULL || name == NULL) {
        printf("Erreur: wt ou name passe en parametre est null [appendWorkTree()]\n");
        return -1;
    }

    if (inWorkTree(wt, name) == -1) { // Si le fichier n'est pas déjà présent dans le WorkTree
        //printf("inWorkTree renvoie:",inWorkTree(wt, name) );
        if (wt->n >= wt->size) {
            // Si le nombre de fichiers dans le WorkTree atteint sa capacité maximale, affiche un message d'erreur et retourne -1
            //printf("Le WorkTree est plein dans appendWorkTree\n");
            return -1;
        }
        // On crée un WorkFile et on le remplit avec les données fournies
        WorkFile * wf = createWorkFile(name);
        if (wf == NULL) {
            printf("Error: couldn't create WorkFile in appendWorkTree()\n");
            return -1;
        }
        wf->hash = strdup(hash);
        wf->mode = mode;
        // Ajoute le WorkFile au WorkTree et incrémente le compteur de fichiers
        wt->tab[wt->n++] = *wf; 
        // Retourne l'indice du nouveau fichier ajouté
        return wt->n - 1;
    }
    return -1;
}

/*
Q 4.7-convertit un WorkTree en une chaîne de caractères composée des
 représentations des WorkFile séparées par un saut de ligne (caractère '\n')
*/
char* wtts(WorkTree* wt) {
    // Vérifie si le pointeur WorkTree est NULL 
    if (!wt) {
        printf("Erreur : pointeur NULL dans wtts\n");
        return NULL;
    }
    // Alloue de la mémoire pour la chaîne résultante
    int buffer_taille = wt->n * 300;
    char *chaine = (char *)malloc(buffer_taille);
    if (!chaine) {
        printf("Erreur lors de l'allocation de memoire dans wtts\n");
        return NULL;
    }

    chaine[0] = '\0';
    // Parcourt les WorkFiles dans le WorkTree pour chaque workfile on applique wtfs et on concatène à la chaine resultat
    for (int i = 0; i < wt->n; i++) {
        if (!wt->tab) {
            printf("Erreur : pointeur NULL dans wtts (tab)\n");
            free(chaine);
            return NULL;
        }
        // Convertit chaque WorkFile en une chaîne de caractères
        char *ligne = wfts(&wt->tab[i]);
        if (!ligne) {
            printf("Erreur : pointeur NULL dans wtts (ligne)\n"); //On vérifie si wfts ne renvoi pas un pointeur null pour eviter les segmentations fault
            free(chaine);
            return NULL;
        }
        // Concatène la chaîne de caractères du WorkFile à la chaîne résultat
        strcat(chaine, ligne);
        // Ajoute un saut de ligne à la chaîne résultat
        strcat(chaine, "\n");
        // Libère la mémoire allouée pour la chaîne de caractères du WorkFile
        free(ligne);
    }
    // Retourne la chaîne résultante
    return chaine;
}


/* Q 4.8-convertit une chaîne de caractères représentant un WorkTree en un WorkTree
*/
WorkTree * stwt ( char * ch){
    if (ch==NULL){
        printf("Erreur:ch est null [stwt] ");
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
    //printf("Avant wt = initWorkTree ()\n");
    WorkTree * wt = initWorkTree ();

    // Boucle pour parcourir la chaîne
    while (pos < strlen (ch)){
        //printf("rentre dans la boucle while avant ptr = strchr (ch + pos , sep )");

        // Recherche du séparateur dans la chaîne
        ptr = strchr (ch + pos , sep );
        //printf("rentre dans la boucle while");

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
    // Si la chaîne n'existe pas on retourne -1
    if (!chaine) {
        printf("Erreur chaine est null [wttf]");
        return -1;
    }

    char path[100];
    // On construit le chemin complet vers le fichier à écrire
    sprintf(path, "TestFiles/%s", fichier);

    FILE *f = fopen(path, "w");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier [wttf]\n");
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



