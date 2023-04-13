#include "../headers/exercice4.h"

int main() {
    WorkTree *wt = initWorkTree();
    appendWorkTree(wt, "file1.txt", "hash1", 0644);
    appendWorkTree(wt, "file2.txt", "hash2", 0755);

    char *wt_str = wtts(wt);
    printf("WorkTree:\n%s\n", wt_str);

    int ret = wttf(wt, "worktree.txt");
    if (ret == 0) {
        printf("WorkTree ecrit dans worktree.txt\n");
    }

    WorkTree *wt_read = ftwt("worktree.txt");
    char *wt_read_str = wtts(wt_read);
    printf("WorkTree lu depuis le fichier:\nTestFiles/%s\n", wt_read_str);

    free(wt_str);
    free(wt_read_str);
    

    return 0;
}