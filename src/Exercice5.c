#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Exercice 5
//Q1
char* hashToFile(char* hash){
    char* ch2 = strdup(hash); 
    ch2[2] = '\0';
    if (stat(ch2, &st) == -1) {
        mkdir(ch2, 0700);
    }
    return hashToPath(hash);
}

void blobWorkTree(WorkTree* wt){
    char fname [100] = "/tmp/myfileXXXXXX"; 
    int fd = mkstemp(fname);
    wttf(wt, fname);
    char* hash = sha256file(fname);
    char* ch = hashToFile(hash);
    strcat(ch, ".t");
    cp(ch, fname);
    return hash;
}

//Q2
char* concat_paths(char* path1, char* path2) {
    char * result = malloc(strlen(path1) + strlen(path2) + 1); 
    if (result == NULL) {
        printf(”Error : unable to allocate memory\n”);
        return NULL; 
    }
    strcpy(result, path1);
    strcat(result, "/");
    strcat(result, path2);
    return result;
}

char* saveWorkTree(WorkTree* wt, char* path){ 
    for (int i=0; i<wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name);
        if (isFile(absPath) == 1){
            blobFile(absPath);
            wt->tab[i].hash = sha256file(absPath); 
            wt->tab[i].mode = getChmod(absPath);
        }else{
            WorkTree* wt2 = initWorkTree();
            List* L = listdir(absPath);
            for (Cell* ptr = *L; ptr != NULL; ptr = ptr->next){
                if(ptr->data[0] == '.') 
                    continue;
                appendWorkTree(wt2, ptr->data, 0, NULL); 
            }
            wt->tab[i].hash = saveWorkTree(wt2, absPath);
            wt->tab[i].mode = getChmod(absPath);
        }
    }
    return blobWorkTree(wt);
}

//Q3
int isWorkTree(char* hash){
    if (file_exists(strcat(hashToPath(hash), ".t"))){
        return 1; 
    }
    if (file_exists(hashToPath(hash))){ 
        return 0;
    }
    return -1;
}

void restoreWorkTree(WorkTree* wt, char* path){
    for(int i=0; i < wt->n; i++){
        char* absPath = concat_paths(path, wt->tab[i].name); 
        char* copyPath = hashToPath(wt->tab[i].hash);
        char* hash = wt->tab[i].hash;
        if (isWorkTree(hash) == 0){
            cp(absPath , copyPath);
            setMode(getChmod(copyPath),absPath); 
        }else{
            if(isWorkTree(hash) == 1){
                strcat(copyPath, ".t");
                WorkTree* nwt = ftwt(copyPath); 
                restoreWorkTree(nwt, absPath); 
                setMode(getChmod(copyPath),absPath);
            }
        }
    }
}

int main(){
    // Test pour la question 1
    char* hash = "a1b2c3d4e5f6";
    char* path = hashToFile(hash);
    printf("Le chemin associé à %s est : %s\n", hash, path);

    // Test pour la question 2
    WorkTree* wt = initWorkTree();
    appendWorkTree(wt, "test1.txt", 0, NULL);
    appendWorkTree(wt, "test2.txt", 0, NULL);
    char* saved = saveWorkTree(wt, "/tmp");
    printf("Le hash associé à l'arbre de travail sauvegardé est : %s\n", saved);

    // Test pour la question 3
    WorkTree* new_wt = initWorkTree();
    restoreWorkTree(new_wt, "/tmp");
    printf("Les fichiers restaurés sont : \n");
    for (int i=0; i<new_wt->n; i++){
        printf("%s\n", new_wt->tab[i].name);
    }

    return 0;
}
