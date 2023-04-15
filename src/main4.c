#include "../headers/exercice4.h"

int main() {
    // Test de createWorkFile :
    printf("Test de createWorkFile :\n");
    WorkFile *wf = createWorkFile("file.txt");
    printf("name = %s, hash = %s, mode = %d\n\n", wf->name, wf->hash, wf->mode);

    // Test de wfts :
    printf("Test de wfts :\n");
    char *chaine = wfts(wf);
    printf("%s\n\n", chaine);
    free(chaine);
    
    //Test de stwf :
    printf("Test de stwf :\n");
    WorkFile *wf2 = stwf("nom:file.txt\thash:abcd1234\tmode:777");
    printf("name = %s, hash = %s, mode = %d\n\n", wf2->name, wf2->hash, wf2->mode);
    free(wf2->name);
    free(wf2->hash);
    free(wf2);
    
    // Test de initWorkTree :
    printf("Test de initWorkTree :\n");
    WorkTree *wt = initWorkTree();
    printf("size = %d, n = %d\n\n", wt->size, wt->n);
    
    // Test de appendWorkTree :
    printf("Test de appendWorkTree :\n");
    appendWorkTree(wt, "file.txt", "abcd1234", 777);
    printf("size = %d, n = %d\n\n", wt->size, wt->n);
    
    // Test de wtts :
    printf("Test de wtts :\n");
    char *chaine2 = wtts(wt);
    printf("%s\n\n", chaine2);
    free(chaine2);

    
    // Test de stwt :
    printf("Test de stwt :\n");
    WorkTree *wt2 = stwt("nom:file.txt\thash:abcd1234\tmode:777\n");
    printf("size = %d, n = %d\n\n", wt2->size, wt2->n);
    
    // Test de wttf :
    printf("Test de wttf :\n");
    int res = wttf(wt, "worktree.txt");
    printf("Résultat de wttf : %d\n\n", res);
    
    // Test de ftwt :
    printf("Test de ftwt :\n");
    WorkTree *wt3 = ftwt("worktree.txt");
    printf("size = %d, n = %d\n\n", wt3->size, wt3->n);
    
    return 0;
}
