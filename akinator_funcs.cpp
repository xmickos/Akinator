#include "akinator.h"

Node* OpNew(Node* node, Elem_t value, FILE* logfile){

    // Можно объявлять Node node = {}; прямо здесь и возвращать через return ?
    // Ответ: можно, если возвращать не указатель на node, а сам node - тогда вернётся не указатель на
    // уже затёртый адрес, а копия всей структуры.

    VERIFICATION_LOGFILE(logfile);
    ECHO(logfile);
    VERIFICATION(node == nullptr, "node pointer is nullptr!", logfile, (Node*)nullptr);

    node = (Node*)calloc(1, sizeof(Node));

    node->data = value;

    node->left = nullptr;
    node->right = nullptr;

    fprintf(logfile, "Created node with val = %d\n", value);

    return node;
}

Node* OpDelete(Node* node, FILE* logfile){

    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(node == nullptr, "Input node is nullptr!", logfile, (Node*)nullptr);
    VERIFICATION(node->left == nullptr, "node->left is nullptr!", logfile, (Node*)nullptr);
    VERIFICATION(node->right == nullptr, "node->right is nullptr!", logfile, (Node*)nullptr);

    if(node->left != nullptr){
        if(!OpDelete(node->left, logfile)){
            fprintf(logfile, "[%s, %d]: Failed to delete left subtree!\nExiting...\n", __FUNCTION__, __LINE__);
            return (Node*)nullptr;
            // Может переписать ? Неясно что с "обрубками" и их памятью будет, если где-то посреди удаления всё сломалось
        }
    }

    if(node->right != nullptr){
        if(!OpDelete(node->right, logfile)){
            fprintf(logfile, "[%s, %d]: Failed to delete right subtree!\nExiting...\n", __FUNCTION__, __LINE__);
            return (Node*)nullptr;
            // Аналогичный вопрос...
        }
    }


    if(node->right != nullptr){
        OpDelete(node->right, logfile);
    }

    free(node->left);
    node->left = nullptr;

    free(node->right);
    node->right = nullptr;

    return 0;
}

Root RootCtor(FILE* logfile){

    VERIFICATION_LOGFILE(logfile);

    Root root = {};
    root.size = DEFAULT_SIZE;
    root.tree_root = (Node*)calloc(1, sizeof(Node));
    if(root.tree_root == nullptr){
        fprintf(logfile, "[%s, %d]: Failed to allocate memory for tree_root!\nExiting...\n", __FUNCTION__, __LINE__);
        return {};
    }

    root.tree_root->data  = DEFAULT_DATA_VALUE;
    root.tree_root->left  = nullptr;
    root.tree_root->right = nullptr;

    return root;
}


int OpInsertSort(Root* root, Elem_t value, FILE* logfile){

    VERIFICATION_LOGFILE(logfile);
    ECHO(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    Node* curr = root->tree_root;

    while(1){
        if(value <= curr->data){
            if(curr->left == nullptr){
                curr->left = OpNew(curr, value, logfile);
                break;
            }
            curr = curr->left;
        }else{
            if(curr->right == nullptr){
                curr->right = OpNew(curr, value, logfile);
                break;
            }
            curr = curr->right;
        }

    }

    return 0;
}

int TreeTextDump(Root* root, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    return 0;
}

int TreePartialTextDump(Node* node, size_t indent, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);

    if(node == nullptr) return 0;

    for(size_t i = 0; i < indent + 1; i++) fprintf(logfile, "\t");
    fprintf(logfile, "data: %d\n", node->data);
    for(size_t i = 0; i < indent + 1; i++) fprintf(logfile, "\t");
    fprintf(logfile, "left:\t\n");
    TreePartialTextDump(node->left, indent + 1, logfile);
    for(size_t i = 0; i < indent + 1; i++) fprintf(logfile, "\t");
    fprintf(logfile, "right:\t\n");
    TreePartialTextDump(node->right, indent + 1, logfile);

    return 0;
}

int Verificator(Root* root, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    // ?

    return 0;
}
