# Q1.2 : 
cat main.c|sha256sum>tmp.txt

# 1.3 :
int hashFile(char* source, char* dest){
    char buffer[256];
    sprintf(buffer,"cat %s|sha256sum>%s",source,dest);
    system(buffer);
    return 1;
}

# 1.4 :

char* sha256file(char* file)  
