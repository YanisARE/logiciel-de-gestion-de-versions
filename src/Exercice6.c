#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Exercice4.c"

typedef struct key_value_pair{
    char* key;
    char* value;
}kvp;

typedef struct hash_table{
    kvp** T;
    int n;
    int size; 
}HashTable;

typedef HashTable Commit;

//Q1
kvp* createKeyVal(char* key, char* val){
    kvp* k = malloc(sizeof(kvp));
    k->key = strdup(key);
    k->value = strdup(val);
    return k;
}

void freeKeyVal(kvp* kv){
    free(kv->key);
    free(kv->value); 
    free(kv);
}

//Q2
char* kvts(kvp* k){
    char* buff = malloc(sizeof(char)*100);
    sprintf(buff, "%s : %s", k->key, k->value);
    return buff;
}

kvp* stkv(char* str){
    char key [100], val [100];
    sscanf(str, "%s : %s", key, val);
    return createKeyVal(key, val);
}

//Q3
Commit* initCommit(){
    Commit* c = malloc(sizeof(Commit));
    c->T = malloc(size*sizeof(kvp*));
    c->size = Ncommit;
    for (int i = 0; i<c->size; i++){
        c->T[i] = NULL;
    }
    c->n = 0;
    return c;
}

//Q4

//Q5
void commitSet(Commit* c, char* key, char* value){
    int p = hashF(key)%c->size;
    while (c->T[p] != NULL){
        p = (p+1)%c->size;
    }
    c->T[p] = createKeyVal(key, value);
    c->n++;
}

//Q6
Commit* createCommit(char* hash){
    Commit* c = initCommit(Ncommit);
    commitSet("tree", hash);
    return c;
}

//Q7
char* commitGet(Commit* c, char* key){
    int p = hashF(key) % c->size;
    int attempt = 0;
    while (c->T[p] != NULL && attempt < c->size) {
        if (strcmp(c->T[p]->key, key) == 0){ 
            return c->T[p]->value;
        }
        p = (p+1)%c->size;
        attempt = attempt +1;
    }
    return NULL;
}

//Q8
char* cts(Commit* c){
    char* str = malloc(sizeof(char)*100*c->n);
    for (int i = 0; i < c->size; i++){
        if(c->T[i] != NULL){
            strcat(str, kvts(c->T[i]));
            strcat(str, ”\n”); }
        }
    return str;
}

Commit* stc(char* ch){ 
    int pos = 0;
    int n_pos = 0;
    int sep = '\n';
    char* ptr;
    char* result = malloc(sizeof(char)*10000); 
    Commit* c = initCommit(10);
    while (pos < strlen(ch)){
        ptr = strchr(ch + pos, sep); 
        if (ptr == NULL){
            n_pos = strlen(ch)+1;
        }
        else{
            n_pos = ptr - ch + 1;
        }
        memcpy(result, ch+pos, n_pos - pos - 1);
        result[n_pos - pos - 1] = '\0';
        pos = n_pos;
        kvp* elem = stkv(result);
        commitSet(c, elem->key, elem->value);
    }
    return c;
}

//Q9
Commit* ftc(char* file){
    char buff[MESSAGE_SIZE];
    char* all = malloc(sizeof(char)*MESSAGE_SIZE);
    FILE* f = fopen(file, "r");
    if (f == NULL){
        printf("ERROR: file does not exist \n");
        return NULL;
    }
    while (fgets(buff, N, f) != NULL){
        strcat(all, buff);
    }
    Commit* c = stc(all);
    return c;
}
void ctf(Commit* c, char* file){
    FILE *fp = fopen(file, "w");
    if (fp != NULL){
        fputs(cts(c), fp);
        fclose(fp);
    }
}

//Q10
char* blobCommit(Commit* c){
    char fname [100] = "/tmp/myfileXXXXXX";
    int fd = mkstemp(fname);
    ctf(c, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash); 
    strcat(ch, ".c");
    cp(ch, fname);
    return hash;
}