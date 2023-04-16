#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/Exercice6.h"


// Q1: Alloue et initialise un élément kvp (key-value pair) contenant deux char * : char* key;char* value; qui est la structure de la table de 
kvp* createKeyVal(char* key, char* val){
    kvp* k = malloc(sizeof(kvp)); // Alloue de la mémoire pour un nouvel élément kvp
    k->key = strdup(key); // Duplique la clé
    k->value = strdup(val); // Duplique la valeur
    return k; // Retourne le pointeur vers le nouvel élément kvp
}

// Q1: Libère la mémoire associée à un élément kvp
void freeKeyVal(kvp* kv){
    free(kv->key); // Libère la mémoire associée à la clé
    free(kv->value); // Libère la mémoire associée à la valeur
    free(kv); // Libère la mémoire associée à la structure kvp
}

// Q2: Convertit un élément kvp en une chaîne de caractères de la forme "clé : valeur"
char* kvts(kvp* k){
    char* buff = malloc(sizeof(char)*100); // Alloue de la mémoire pour la chaîne de caractères
    sprintf(buff, "%s : %s", k->key, k->value); // Écrit la chaîne de caractères sous la forme "clé : valeur"
    return buff; // Retourne la chaîne de caractères formatée
}

// Q2: Conversion inverse de kvts (string to key-value pair)
kvp* stkv(char* str){
    char key [100], val [100]; // Alloue de la mémoire pour stocker la clé et la valeur
    sscanf(str, "%s : %s", key, val); // Extrait la clé et la valeur de la chaîne de caractères
    return createKeyVal(key, val); // Crée et retourne un nouvel élément kvp à partir de la clé et la valeur
}

// Q3: Alloue et initialise un Commit de taille fixée
Commit* initCommit(){
    Commit* c = malloc(sizeof(Commit)); // Alloue de la mémoire pour un nouvel élément Commit
    c->size = 100; // Initialise la taille du Commit
    c->T = malloc(c->size*sizeof(kvp*)); // Alloue de la mémoire pour le tableau de pointeurs kvp
    for (int i = 0; i<c->size; i++){
        c->T[i] = NULL; // Initialise chaque élément du tableau à NULL
    }
    c->n = 0; // Initialise le nombre d'éléments à 0
    return c; // Retourne le pointeur vers le nouvel élément Commit
}

// Q4: Fonction de hashage djb2
unsigned long djb2(unsigned char *str){
    unsigned long hash = 5381; // Initialise la valeur du hash
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c; // Calcule le hash avec la formule: hash * 33 + c
    }
    return hash; // Retourne le hash calculé
}

// Q5: Insère la paire (key, value) dans la table, en gérant les collisions par adressage ouvert et probing linéaire
void commitSet(Commit* c, char* key, char* value){
    int p = djb2((unsigned char *)key) % c->size; // Calcule la position initiale dans la table à l'aide de la fonction de hashage
    while (c->T[p] != NULL){ // Si la position est occupée, recherche une position libre avec probing linéaire
        p = (p+1)%c->size; // Probing linéaire: augmente la position de 1 (mod taille de la table)
    }
    c->T[p] = createKeyVal(key, value); // Insère la paire (key, value) à la position trouvée
    c->n++; // Incrémente le nombre d'éléments dans la table
}

// Q6: Alloue et initialise un Commit, puis ajoute l'élément obligatoire correspondant à la clé "tree"
Commit* createCommit(char* hash, int Ncommit){
    Commit* c = initCommit(Ncommit); // Initialise un nouvel élément Commit
    commitSet(c, "tree", hash); // Ajoute l'élément obligatoire avec la clé "tree"
    return c; // Retourne le pointeur vers le nouvel élément Commit
}

// Q7: Cherche dans la table s'il existe un élément dont la clé est key (en sachant que les conflits sont résolus par adressage ouvert et probing linéaire)
char* commitGet(Commit* c, char* key){
    int p = djb2((unsigned char *)key) % c->size; // Calcule la position initiale dans la table à l'aide de la fonction de hashage
    int attempt = 0;
    while (c->T[p] != NULL && attempt < c->size) { // Parcourt la table jusqu'à trouver un emplacement vide ou atteindre la fin
        if (strcmp(c->T[p]->key, key) == 0){ // Si la clé correspond à la clé recherchée
            return c->T[p]->value; // Retourne la valeur associée à la clé
        }
        p = (p+1)%c->size; // Probing linéaire: augmente la position de 1 (mod taille de la table)
        attempt = attempt +1; // Incrémente le compteur d'essais
    }
    return NULL; // Si la clé n'est pas trouvée, retourne NULL
}

// Q8: Crée une chaîne de caractères représentant chacun de ses couples (clé, valeur), séparées par un saut de ligne
char* cts(Commit* c){
    char* str = malloc(sizeof(char)*100*c->n); // Alloue de la mémoire pour la chaîne de caractères
    for (int i = 0; i < c->size; i++){ // Parcourt le tableau de pointeurs kvp
        if(c->T[i] != NULL){ // Si l'élément n'est pas NULL
            strcat(str, kvts(c->T[i])); // Ajoute la représentation sous forme de chaîne de caractères de l'élément kvp à la chaîne str
            strcat(str, "\n"); // Ajoute un saut de ligne à la chaîne str
        }
    }
    return str; // Retourne la chaîne de caractères
}

// Réalise la conversion inverse de cts (Commit to String)
Commit* stc(char* ch){
    size_t pos = 0;
    size_t n_pos = 0;
    int sep = '\n';
    char* ptr;
    char* result = malloc(sizeof(char)*10000); // Alloue de la mémoire pour la chaîne de caractères résultante
    Commit* c = initCommit(10); // Initialise un nouvel élément Commit
    while (pos < strlen(ch)){ // Parcourt la chaîne de caractères ch
        ptr = strchr(ch + pos, sep); // Trouve la position du prochain séparateur
        if (ptr == NULL){
            n_pos = strlen(ch)+1;
        }
        else{
            n_pos = ptr - ch + 1;
        }
        memcpy(result, ch+pos, n_pos - pos - 1); // Copie le segment entre les séparateurs dans result
        result[n_pos - pos - 1] = '\0'; // Ajoute le caractère de fin de chaîne
        pos = n_pos; // Met à jour la position de départ pour la prochaine itération
        kvp* elem = stkv(result); // Convertit la chaîne résultante en un élément kvp
        commitSet(c, elem->key, elem->value); // Insère l'élément kvp dans le Commit
    }
    return c; // Retourne le pointeur vers le nouvel élément Commit
}



#define N 100
//permet de charger un Commit depuis un fichier le représentant
Commit* ftc(char* file){
    char buff[256];
    char* all = (char *) malloc(sizeof(char)*256);
    FILE* f = fopen(file, "r"); // Ouvre le fichier en mode lecture
    if (f == NULL){ // Si le fichier n'existe pas
        printf("Erreur: le fichier n'existe pas ftc \n");
        return NULL;
    }
    while (fgets(buff, N, f) != NULL){ // Lit le contenu du fichier ligne par ligne
        strcat(all, buff); // Ajoute la ligne lue à la chaîne all
    }
    Commit* c = stc(all); // Convertit la chaîne all en un objet Commit
    return c; // Retourne le pointeur vers le nouvel élément Commit
}


// Q9: Écrit dans le fichier file la chaîne de caractères représentant le commit c
void ctf(Commit* c, char* file){
    FILE *fp = fopen(file, "w"); // Ouvre le fichier en mode écriture
    if (fp != NULL){
        fputs(cts(c), fp); // Écrit la chaîne de caractères représentant le commit c dans le fichier
        fclose(fp); // Ferme le fichier
    }
}

// Q10: Crée un enregistrement instantané d’un Commit en passant par un fichier temporaire. Retourne le hash du fichier temporaire
char* blobCommit(Commit* c){
    char fname[100] = "/tmp/myfileXXXXXX"; // Crée un nom de fichier temporaire
    int fd = mkstemp(fname); // Génère un fichier temporaire unique
    ctf(c, fname); // Écrit le commit dans le fichier temporaire
    char* hash = sha256file(fname); // Calcule le hash SHA256 du fichier temporaire
    char* ch = hashToFile(hash); // Convertit le hash en nom de fichier
    strcat(ch, ".c"); // Ajoute l'extension ".c" au nom de fichier
    cp(ch, fname); // Copie le fichier temporaire dans le fichier final
    return hash; // Retourne le hash du fichier temporaire
}


