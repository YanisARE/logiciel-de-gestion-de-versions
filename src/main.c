#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


//1.3:
int hashFile(char* source, char* dest){
    char buffer[256];
    sprintf(buffer,"cat %s|sha256sum>%s",source,dest);
    system(buffer);
    return 1;
}

//1.4:
char* sha256file(char* file){   
    static char template [] = "/tmp/myfileXXXXXXX" ;
    char fname [1000];
    strcpy ( fname , template ) ;
    int val_retour=hashFile(file, fname);
    printf("Le hachage s'est bien passee : %d\n",val_retour);
    char hash[256];
    FILE *f=fopen(fname,"r");
    fgets(hash,256,f);
    fclose(f);
    unlink(fname);
    char *resultat = malloc(sizeof(hash));
    strcpy(resultat, hash);
    return resultat;
}



