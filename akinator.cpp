#include "akinator.h"

int main(){

    FILE* logfile = fopen("logfile.txt", "w");
    FILE* dotfile = fopen("my_dotcode.dot", "w");
    FILE* backupfile = fopen("tree_backup.txt", "r");
    Root root = {};
    RootCtor(&root, logfile);

    printf("root.size = %zu\n", root.size);

    printf("data: %d\n", root.tree_root->data);

    // OpInsertSort(&root, 10, logfile);
    OpInsertSort(&root, 5, logfile);
    OpInsertSort(&root, 15, logfile);
    OpInsertSort(&root, 12, logfile);
    OpInsertSort(&root, 20, logfile);
    OpInsertSort(&root, 11, logfile);

    TreePartialTextDump(root.tree_root, 0, logfile);

    OpGraphDump(&root, dotfile, logfile);

    PrintNode(root.tree_root, logfile);

    ReadTree(backupfile, root.tree_root->left, logfile);

    fclose(logfile);
    fclose(dotfile);
    fclose(backupfile);
    return 0;
}


/*
line  4
line 31
line 39

*/
