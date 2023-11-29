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
    // OpInsertSort(&root, 5, logfile);
    // OpInsertSort(&root, 15, logfile);
    // OpInsertSort(&root, 12, logfile);
    // OpInsertSort(&root, 20, logfile);
    // OpInsertSort(&root, 11, logfile);

    root.tree_root->left = (Node*)calloc(1, sizeof(Node));
    root.tree_root->left->left = (Node*)calloc(1, sizeof(Node));
    root.tree_root->left->right = nullptr;
    root.tree_root->left->left->data = 11;
    root.tree_root->left->left->left = nullptr;
    root.tree_root->left->left->right = nullptr;

    root.tree_root->right = (Node*)calloc(1, sizeof(Node));
    root.tree_root->right->left = nullptr;
    root.tree_root->right->right = (Node*)calloc(1, sizeof(Node));
    root.tree_root->right->right->left = nullptr;
    root.tree_root->right->right->right = nullptr;
    root.tree_root->right->right->data = 14;




    TreePartialTextDump(root.tree_root, 0, logfile);

    printf("\n\n-------------------------\n\n");

    TreePartialTextDump(root.tree_root, 0, stdout);

    OpGraphDump(&root, dotfile, logfile);

    PrintNode(root.tree_root, logfile);

    ReadTree(backupfile, root.tree_root->left, logfile);

    fprintf(logfile, "\na\n");

    TreePartialTextDump(root.tree_root, 0, logfile);

    TreePartialTextDump(root.tree_root, 0, stdout);

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
