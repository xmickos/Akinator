#include "akinator.h"


Node* OpNew(Elem_t value, FILE* logfile){

    // Можно объявлять Node node = {}; прямо здесь и возвращать через return ?
    // Ответ: можно, если возвращать не указатель на node, а сам node - тогда вернётся не указатель на
    // уже затёртый адрес, а копия всей структуры.

    VERIFICATION_LOGFILE(logfile, nullptr);
    ECHO(logfile);

    Node* node = (Node*)calloc(1, sizeof(Node));
    node->data = (char*)calloc(DEFAULT_SIZE, sizeof(char));

    strcpy(node->data, value);
    node->left = nullptr;
    node->right = nullptr;

    fprintf(logfile, "Created node with data:  %s\n", value);

    return node;
}

Node* OpDelete(Node* node, FILE* logfile){

    VERIFICATION_LOGFILE(logfile, nullptr);
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

    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    // TODO: pass &root as an argument
    root->size = 1;
    root->tree_root = (Node*)calloc(1, sizeof(Node));
    if(root->tree_root == nullptr){
        fprintf(logfile, "[%s, %d]: Failed to allocate memory for tree_root!\nExiting...\n", __FUNCTION__, __LINE__);
        return -1;
    }

    root->tree_root->data  = (char*)calloc(4 * DEFAULT_SIZE, sizeof(char));
    strcpy(root->tree_root->data, "Unauthorized access to this device is prohibited!");
    root->tree_root->left  = nullptr;
    root->tree_root->right = nullptr;

    return 0;
}

/*
int OpInsertSort(Root* root, Elem_t value, FILE* logfile){

    VERIFICATION_LOGFILE(logfile, -1);
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
*/

int TreePartialTextDump(Node* node, size_t indent, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);

    if(node == nullptr) return 0;

    IndentMe(indent + 1u, logfile);
    fprintf(logfile, "data: %s\n", node->data);

    IndentMe(indent + 1u, logfile);
    fprintf(logfile, "left:\t\n");
    TreePartialTextDump(node->left, indent + 1, logfile);

    IndentMe(indent + 1u, logfile);
    fprintf(logfile, "right:\t\n");
    TreePartialTextDump(node->right, indent + 1, logfile);

    return 0;
}

int Verificator(Root* root, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    // ?

    return 0;
}

int OpGraphDump(Root* root, FILE* dotfile, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    VERIFICATION(dotfile == nullptr, "Input dotfile is nullptr!", logfile, -1);

    fprintf(dotfile, GRAPHVIZ_INIT);

    fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%d"), 0, 0, 0, 0, 0);

    OpPartialGraphDump(root->tree_root, 0, (char)0, dotfile, logfile);

    fprintf(dotfile, GRAPHVIZ_END);

    return 0;
}

int OpPartialGraphDump(Node* node, unsigned char ip, unsigned char depth, FILE* dotfile, FILE* logfile){

    VERIFICATION_LOGFILE(logfile, -1);
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
        fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%s"), ip, new_depth, node->left->data, ip, new_depth);
        fprintf(dotfile, GRAPHVIZ_CONNECT_NODE("\"%d/%d\"", f1, "\"%d/%d\"", f0), ip, depth, ip, new_depth);
    }


    if(node->right != nullptr){
        // при создании узла right его ip получается из головного узла данной подсети установкой первого ранее незначащего бита
        // в единицу - 10100000/3 становится 10110000/4
        // 10110000 = 10100000 | 00010000 = 10100000 | (1 << 4) = 10100000 | (1 << (CHAR_BIT - (depth + 1)) =
        // = 10100000 | (1 << (CHAR_BIT - depth - 1));
        // Итого создаётся новый узел с именем 10110000/4 == 174/4
        new_ip = ip | (unsigned char)(1 << (CHAR_BIT - depth - 1));
        fprintf(dotfile, GRAPHVIZ_MKNODE("%d/%d", "%s"), new_ip, new_depth, node->right->data, new_ip, new_depth);
        fprintf(dotfile, GRAPHVIZ_CONNECT_NODE("\"%d/%d\"", f2, "\"%d/%d\"", f0), ip, depth, new_ip, new_depth);
    }

    OpPartialGraphDump(node->left, ip, new_depth, dotfile, logfile);
    OpPartialGraphDump(node->right, new_ip, new_depth, dotfile, logfile);

    return 0;
}

void IndentMe(size_t count, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, );
    for(size_t i = 0; i < count; i++) fprintf(logfile, "\t");
}

void PrintNode(const Node* node, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, );

    if(!node){
        fprintf(logfile, "nil");
        return;
    }

    fprintf(logfile, "(");
    fprintf(logfile, "\"%s\"", node->data);
    PrintNode(node->left, logfile);
    PrintNode(node->right, logfile);
    fprintf(logfile, ")");

}

int ReadTree(FILE* backupfile, Node* init_node, FILE* logfile){
    // Пока функция "приклеивает" прочитанное дерево только в правое поддерево данной ноды, можно переписать в дальнейшем.

    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(backupfile == nullptr, "Input backupfile is nullptr!", logfile, -1);
    VERIFICATION(init_node == nullptr, "Input init_node is nullptr!", logfile, -1);

    struct stat filestat;
    size_t filesize = 0;
    int pos = 0;

    stat("./tree_backup.txt", &filestat);
    filesize = filestat.st_size;

    char* buff = (char*)calloc(filesize, sizeof(char));
    VERIFICATION(buff == nullptr, "Bad calloc!", logfile, -1);

    if(fread(buff, sizeof(char), filesize - 1, backupfile) < 0){
        fprintf(logfile, "Bad fileread!\n\n");
        return -1;
    }
    buff[filesize - 1] = '\0';

    printf("size = %lld\n", filestat.st_size);
    printf("buff: %s\n", buff);

    init_node->right = PartialTreeRead(buff, &pos, logfile);

    printf("Bye:)\n");

    return 0;
}


Node* PartialTreeRead(char* buff, int* pos, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, nullptr);
    VERIFICATION(pos == nullptr, "Lost pos address!", logfile, nullptr);

    int offset = 0;
    char command_buff[DEFAULT_SIZE] = {}, node_val[DEFAULT_SIZE] = {};

    printf("Another level...\n");

    if(buff[*pos] != '('){
        printf("Bad syntax. Remaining buff: %s\n", buff + *pos);
        return nullptr;
    }else{
        (*pos)++;
        printf("Increasing pos...\nbuff: %s\n", buff + *pos);
    }

    Node* node = OpNew("\0", logfile);

    while(buff[*pos + 1] != '"'){
        (*pos)++;
        node_val[offset++] = buff[*pos];
    }

    node_val[offset + 1] = '\0';

    printf("Value! :3 \"%s\"!\n", node_val);
    strcpy(node->data, node_val);
    *pos += 2; // +2 for remaining ' " ' and last symbol in prev string.
    printf("New pos is %d\n", *pos);

    if(buff[*pos] == '('){
        printf("Going deeper left...\n");
        node->left = PartialTreeRead(buff, pos, logfile);
    }else{


// ("Unauthorized access to this device is prohibited!"("Hello!"("sample text :/"nilnil)nil)("Hola!"nil("Privet!"nilnil)))


        sscanf(buff + *pos, "%3s", command_buff);
        if(!strcmp(command_buff, "nil")){
            printf("Found nil!\n");
            node->left = nullptr;
            *pos += 3;
        }else{
            if(buff[*pos] == '('){
            node->left = PartialTreeRead(buff, pos, logfile);
            }
        }

    }

    sscanf(buff + *pos, "%3s", command_buff);
    if(!strcmp(command_buff, "nil")){
        node->right = nullptr;
        *pos += 3;
        printf("Found nil!\nleft buff: %s\n\n", buff + *pos);
    }else{
        if(buff[*pos] == '('){
        node->right = PartialTreeRead(buff, pos, logfile);
        }
    }

    if(buff[*pos] == ')'){
        (*pos)++;
        printf("Foud closing bracket!\nleft buff: %s\n", buff + *pos);
        return node;
    }

    return nullptr;
}

