#include "akinator.h"

Node* OpNew(Elem_t value, FILE* logfile){

    // Можно объявлять Node node = {}; прямо здесь и возвращать через return ?
    // Ответ: можно, если возвращать не указатель на node, а сам node - тогда вернётся не указатель на
    // уже затёртый адрес, а копия всей структуры.

    VERIFICATION_LOGFILE(logfile);
    ECHO(logfile);

    Node* node = (Node*)calloc(1, sizeof(Node));

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
        }
    }

    if(node->right != nullptr){
        if(!OpDelete(node->right, logfile)){
            fprintf(logfile, "[%s, %d]: Failed to delete right subtree!\nExiting...\n", __FUNCTION__, __LINE__);
            return (Node*)nullptr;
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

int RootCtor(Root* root, FILE* logfile){

    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    // TODO: pass &root as an argument
    root->size = DEFAULT_SIZE;
    root->tree_root = (Node*)calloc(1, sizeof(Node));
    if(root->tree_root == nullptr){
        fprintf(logfile, "[%s, %d]: Failed to allocate memory for tree_root!\nExiting...\n", __FUNCTION__, __LINE__);
        return -1;
    }

    root->tree_root->data  = DEFAULT_DATA_VALUE;
    root->tree_root->left  = nullptr;
    root->tree_root->right = nullptr;

    return 0;
}


int OpInsertSort(Root* root, Elem_t value, FILE* logfile){

    VERIFICATION_LOGFILE(logfile);
    ECHO(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    Node* curr = root->tree_root;

    while(1){
        if(value <= curr->data){
            if(curr->left == nullptr){
                curr->left = OpNew(value, logfile);
                break;
            }
            curr = curr->left;
        }else{
            if(curr->right == nullptr){
                curr->right = OpNew(value, logfile);
                break;
            }
            curr = curr->right;
        }

    }

    return 0;
}

int TreePartialTextDump(Node* node, size_t indent, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);

    if(node == nullptr) return 0;

    IndentMe(indent + 1u, logfile);
    fprintf(logfile, "data: %d\n", node->data);

    IndentMe(indent + 1u, logfile);
    fprintf(logfile, "left:\t\n");
    TreePartialTextDump(node->left, indent + 1, logfile);

    IndentMe(indent + 1u, logfile);
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

int OpGraphDump(Root* root, FILE* dotfile, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    VERIFICATION(dotfile == nullptr, "Input dotfile is nullptr!", logfile, -1);

    fprintf(dotfile, GRAPHVIZ_INIT);

    fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%d"), 0, 0, 0, 0, 0);

    OpPartialGraphDump(root->tree_root, 0, (char)0, dotfile, logfile);

    fprintf(dotfile, GRAPHVIZ_END);

    return 0;
}

int OpPartialGraphDump(Node* node, unsigned char ip, unsigned char depth, FILE* dotfile, FILE* logfile){

    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(dotfile == nullptr, "Input dotfile is nullptr!", logfile, -1);

    unsigned char new_ip = 0, new_depth = depth + (unsigned char)1;

    if(node == nullptr) return 0;

    // (assuming CHAT_BIT == 8)
    // char subnet_mask = ~((1 << (CHAR_BIT - depth)) - 1);
    // 0b 11100000
    // 0b 00011111
    // 0b 00100000
    // 0b 1 << 5 = 1 << (CHAR_BIT - depth);
    //
    // Пример адреса головного node'a этой подсети: 10100000 - валидны первые 3 бита
    //

    if(node->left != nullptr){
        // при создании узла left его ip можно не менять - это означало бы добавить в конец 0
        // при условии, что ip инициализируется нулём.
        // Итого создаётся новый узел с именем 10100000/4 == 160/4
        fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%d"), ip, new_depth, node->left->data, ip, new_depth);
        fprintf(dotfile, GRAPHVIZ_CONNECT_NODE("\"%d/%d\"", f1, "\"%d/%d\"", f0), ip, depth, ip, new_depth);
    }


    if(node->right != nullptr){
        // при создании узла right его ip получается из головного узла данной подсети установкой первого ранее незначащего бита
        // в единицу - 10100000/3 становится 10110000/4
        // 10110000 = 10100000 | 00010000 = 10100000 | (1 << 4) = 10100000 | (1 << (CHAR_BIT - (depth + 1)) =
        // = 10100000 | (1 << (CHAR_BIT - depth - 1));
        // Итого создаётся новый узел с именем 10110000/4 == 174/4
        new_ip = ip | (unsigned char)(1 << (CHAR_BIT - depth - 1));
        fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%d"), new_ip, new_depth, node->right->data, new_ip, new_depth);
        fprintf(dotfile, GRAPHVIZ_CONNECT_NODE("\"%d/%d\"", f2, "\"%d/%d\"", f0), ip, depth, new_ip, new_depth);
    }

    OpPartialGraphDump(node->left, ip, new_depth, dotfile, logfile);
    OpPartialGraphDump(node->right, new_ip, new_depth, dotfile, logfile);

    return 0;
}

void IndentMe(size_t count, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);
    for(size_t i = 0; i < count; i++) fprintf(logfile, "\t");
}

void PrintNode(const Node* node, FILE* logfile){
    VERIFICATION_LOGFILE(logfile);

    if(!node){
        fprintf(logfile, "nil");
        return;
    }

    fprintf(logfile, "(");
    fprintf(logfile, "%d", node->data);
    PrintNode(node->left, logfile);
    PrintNode(node->right, logfile);
    fprintf(logfile, ")");

}

int ReadTree(FILE* backupfile, Node* init_node, FILE* logfile){
    // Пока функция "приклеивает" прочитанное дерево только в правое поддерево данной ноды, можно переписать в дальнейшем.

    VERIFICATION_LOGFILE(logfile);
    VERIFICATION(backupfile == nullptr, "Input backupfile is nullptr!", logfile, -1);
    VERIFICATION(init_node == nullptr, "Input init_node is nullptr!", logfile, -1);

    struct stat buff;
    size_t filesize = 0;

    stat("./tree_backup.txt", &buff);
    filesize = buff.st_size;

    printf("size = %lld\n", buff.st_size);

    return 0;
}
