#include "akinator.h"

int main(){

    FILE* logfile = fopen("logfile.txt", "w");
    FILE* dotfile = fopen("my_dotcode.dot", "w");
    FILE* backupfile = fopen("tree_backup.txt", "r");
    Root root = {};
    RootCtor(&root, logfile);

    printf("root.size = %zu\n", root.size);

    printf("data: %s\n", root.tree_root->data);

//     root.tree_root->left = (Node*)calloc(1, sizeof(Node));
//     root.tree_root->left->left = (Node*)calloc(1, sizeof(Node));
//     strcpy(root.tree_root->left->left->data, "sample text");
//
//     root.tree_root->right = (Node*)calloc(1, sizeof(Node));
//     root.tree_root->right->right = (Node*)calloc(1, sizeof(Node));
//     strcpy(root.tree_root->right->right->data, "sample text");
//
//     root.tree_root->left = OpNew("Привет!", logfile);
//     root.tree_root->left->left = OpNew("sample text :/", logfile);
//     root.tree_root->left->right = nullptr;
//
//     root.tree_root->right = OpNew("Hola!", logfile);
//     root.tree_root->right->right = OpNew("Privet!", logfile);


    // TreePartialTextDump(root.tree_root, 0, logfile);

    // printf("\n\n-------------------------\n\n");

    // TreePartialTextDump(root.tree_root, 0, stdout);

    // OpGraphDump(&root, dotfile, logfile);

    // PrintNode(root.tree_root, logfile);

    // fprintf(logfile, "\n");

    // ReadTree(backupfile, root.tree_root->left, logfile);

    // fprintf(logfile, "\na\n");

    // TreePartialTextDump(root.tree_root, 0, logfile);

    // TreePartialTextDump(root.tree_root, 0, stdout);

    // OpGraphDump(&root, dotfile, logfile);



    char key[DEFAULT_SIZE] = {};

    // scanf("%s", key);
    // printf("Введено: %s\n", key);

    FILE* databasefile = fopen("akinator_database.txt", "r");

    int read_output = ReadTree(databasefile, root.tree_root, logfile);

    printf("read outputted: %d\n", read_output);

    fclose(databasefile);



    AkinatorGuessing(&root, root.tree_root, logfile);






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
