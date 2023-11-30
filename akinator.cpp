#include "akinator.h"

int main(){

    FILE* logfile = fopen("logfile.txt", "w");
    FILE* dotfile = fopen("my_dotcode.dot", "w");
    FILE* backupfile = fopen("tree_backup.txt", "r");
    Root root = {};
    RootCtor(&root, logfile);

    printf("root.size = %zu\n", root.size);

    printf("data: %s\n", root.tree_root->data);

    // OpInsertSort(&root, 10, logfile);
    // OpInsertSort(&root, 5, logfile);
    // OpInsertSort(&root, 15, logfile);
    // OpInsertSort(&root, 12, logfile);
    // OpInsertSort(&root, 20, logfile);
    // OpInsertSort(&root, 11, logfile);

//     root.tree_root->left = (Node*)calloc(1, sizeof(Node));
//     root.tree_root->left->left = (Node*)calloc(1, sizeof(Node));
//     strcpy(root.tree_root->left->left->data, "sample text");
//
//     root.tree_root->right = (Node*)calloc(1, sizeof(Node));
//     root.tree_root->right->right = (Node*)calloc(1, sizeof(Node));
//     strcpy(root.tree_root->right->right->data, "sample text");

    root.tree_root->left = OpNew("Hello!", logfile);
    root.tree_root->left->left = OpNew("sample text :/", logfile);
    root.tree_root->left->right = nullptr;

    root.tree_root->right = OpNew("Hola!", logfile);
    root.tree_root->right->right = OpNew("Privet!", logfile);


    TreePartialTextDump(root.tree_root, 0, logfile);

    printf("\n\n-------------------------\n\n");

    TreePartialTextDump(root.tree_root, 0, stdout);

    OpGraphDump(&root, dotfile, logfile);

    PrintNode(root.tree_root, logfile);

    fprintf(logfile, "\n");

    ReadTree(backupfile, root.tree_root->left, logfile);

    fprintf(logfile, "\na\n");

    TreePartialTextDump(root.tree_root, 0, logfile);

    TreePartialTextDump(root.tree_root, 0, stdout);

    OpGraphDump(&root, dotfile, logfile);

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
