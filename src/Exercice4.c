#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


typedef struct {
    char * name ;
    char * hash ;
    int mode ;
} WorkFile ;

typedef struct {
    WorkFile * tab ;
    int size ;
    int n ;
} WorkTree ;

int getChmod(const char* path) {
    struct stat ret;
    /*vérifie si la fonction stat() (qui récupère les informations sur un fichier, telles que les permissions, la taille, etc.) 
    a réussi à récupérer les informations sur le fichier spécifié par le chemin path et stocke ces informations dans la structure ret.*/
    if (stat(path, &ret) == -1) {
        return -1;
    }
    //constantes S_IRGRP, S_IWGRP et S_IXGRP qui représentent respectivement les permissions de lecture, d'écriture et d'exécution
    return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | /* vérifie le statut du propriétaire du fichier */
           (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) | /* vérifie les permissions du groupe associé au fichier */
           (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH); /*  vérifie les permissions pour les utilisateurs autres que le propriétaire et le groupe associé au fichier */
}

void setMode(int mode, char* path) {
    char buff[100];
    sprintf(buff, "chmod %o %s", mode, path);
    system(buff);
}
// Q 4.1
WorkFile* createWorkFile(char* name){
    WorkFile * wf= (WorkFile * ) malloc(sizeof(WorkFile));
    if (!wf) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }
    wf->name=strdup(name);
    wf->hash=NULL;
    wf->mode=0;
    return wf;
}
// Q 4.2
char* wfts(WorkFile* wf) {
    int longnom = strlen(wf->nom);
    int longhach = strlen(wf->hachage);
    char* chaine = (char*)malloc(longnom + longhach + 12);

    sprintf(chaine, "%s\t%s\t%d", wf->nom, wf->hachage, wf->mode);

    return chaine;
}

// Q 4.3
WorkFile* stwf(char* ch) {
    WorkFile *wf = (WorkFile *)malloc(sizeof(WorkFile));
    if (!wf) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    wf->nom = (char *)malloc(256);
    wf->hachage = (char *)malloc(41);

    sscanf(ch, "nom:%s\thach:%s\tmode:%d", wf->nom, wf->hachage, &wf->mode);

    return wf;
}

// Q 4.4
#define TAILLE_FIXE 100
WorkTree* initWorkTree() {
    WorkTree *wt = (WorkTree *)malloc(sizeof(WorkTree));
    if (!wt) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    wt->tab = (WorkFile *)calloc(TAILLE_FIXE, sizeof(WorkFile));
    wt->taille = TAILLE_FIXE;
    wt->n = 0;

    return wt;
}

// Q 4.5
int inWorkTree(WorkTree* wt, char* nom) {
    for (int i = 0; i < wt->n; i++) {
        if (strcmp(wt->tab[i].nom, nom) == 0) {
            return i;
        }
    }

    return -1;
}

// Q 4.6
int appendWorkTree(WorkTree* wt, char* nom, char* hachage, int mode) {
    if (inWorkTree(wt, nom) == -1) {
        if (wt->n >= wt->taille) {
            printf("Le WorkTree est plein\n");
            return -1;
        }

        WorkFile *wf = &wt->tab[wt->n++];
        wf->nom = strdup(nom);
        wf->hachage = strdup(hachage);
        wf->mode = mode;

        return wt->n - 1;
    }

    return -1;
}

// Q 4.7

char* wtts(WorkTree* wt) {
    char *chaine = (char *)calloc(wt->n * (256 + 41 + 12), sizeof(char));
    int i;
    for (i = 0; i < wt->n; i++) {
        char *ligne = wfts(&wt->tab[i]);
        strcat(chaine, ligne);
        free(ligne);
        if (i < wt->n - 1) {
            strcat(chaine, "\n");
        }
    }

    return chaine;
}

// Q 4.8
WorkTree* stwt(char* ch) {
    WorkTree *wt = initWorkTree();
    if (!wt) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    char *ligne = strtok(ch, "\n");
    while (ligne != NULL) {
        WorkFile *wf = stwf(ligne);
        appendWorkTree(wt, wf->nom, wf->hachage, wf->mode);
        ligne = strtok(NULL, "\n");
    }

    return wt;
}

// Q 4.9
int wttf(WorkTree* wt, char* fichier) {
    FILE *f = fopen(fichier, "w");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return -1;
    }

    char *chaine = wtts(wt);
    fprintf(f, "%s", chaine);

    fclose(f);
    free(chaine);

    return 0;
}

// Q 4.10
WorkTree* ftwt(char* fichier) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long taille = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *chaine = (char *)malloc(taille + 1);
    fread(chaine, 1, taille, f);
    chaine[taille] = '\0';

    fclose(f);

    WorkTree *wt = stwt(chaine);

    free(chaine);

    return wt;
}

int main(int argc, char **argv) {
    WorkTree *wt = initWorkTree();
    appendWorkTree(wt, "file1.txt", "hash1", 0644);
    appendWorkTree(wt, "file2.txt", "hash2", 0755);

    char *wt_str = wtts(wt);
    printf("WorkTree:\n%s\n", wt_str);

    int ret = wttf(wt, "worktree.txt");
    if (ret == 0) {
        printf("WorkTree écrit dans le fichier worktree.txt\n");
    }

    WorkTree *wt_read = ftwt("worktree.txt");
    char *wt_read_str = wtts(wt_read);
    printf("WorkTree lu depuis le fichier:\n%s\n", wt_read_str);

    free(wt_str);
    free(wt_read_str);
    freeWorkTree(wt);
    freeWorkTree(wt_read);

    return 0;
}