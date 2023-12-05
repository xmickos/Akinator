#include "akinator.h"


Node* OpNew(Elem_t value, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, nullptr);

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
    root->size = 1;
    root->tree_root = (Node*)calloc(1, sizeof(Node));
    if(root->tree_root == nullptr){
        fprintf(logfile, "[%s, %d]: Failed to allocate memory for tree_root!\nExiting...\n", __FUNCTION__, __LINE__);
        return -1;
    }

    root->tree_root->data  = (char*)calloc(4 * DEFAULT_SIZE, sizeof(char));
    strcpy(root->tree_root->data, "Неизвестно кто");
    root->tree_root->left  = nullptr;
    root->tree_root->right = nullptr;

    return 0;
}

int TreePartialTextDump(Node* node, size_t indent, FILE* dstfile, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION_LOGFILE(dstfile, -1);

    if(node == nullptr) return 0;

    IndentMe(indent + 1u, dstfile);
    fprintf(dstfile, "data: %s\n", node->data);

    IndentMe(indent + 1u, dstfile);
    fprintf(dstfile, "left:\t\n");
    TreePartialTextDump(node->left, indent + 1, dstfile, logfile);

    IndentMe(indent + 1u, dstfile);
    fprintf(dstfile, "right:\t\n");
    TreePartialTextDump(node->right, indent + 1, dstfile, logfile);

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
    ECHO(logfile);
    // Пока функция "приклеивает" прочитанное дерево только в правое поддерево данной ноды, можно переписать в дальнейшем.

    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(backupfile == nullptr, "Input backupfile is nullptr!", logfile, -1);
    VERIFICATION(init_node == nullptr, "Input init_node is nullptr!", logfile, -1);

    struct stat filestat;
    size_t filesize = 0;
    int pos = 0;

    stat("akinator_database.txt", &filestat);
    filesize = (size_t)filestat.st_size;

    char* buff = (char*)calloc(filesize + 1, sizeof(char));
    VERIFICATION(buff == nullptr, "Bad calloc!", logfile, -1);

    if(fread(buff, sizeof(char), filesize, backupfile) < 0){
        fprintf(logfile, "Bad fileread!\n\n");
        return -1;
    }

    if(filesize > 0){
        buff[filesize] = '\0';
    }

    printf("buff: %s\n", buff);

    init_node->right = PartialTreeRead(buff, &pos, logfile);
    VERIFICATION(init_node->right == nullptr, "Failed to read subtree!", logfile, -1);

    printf("output: %p,\nBye:)\n", init_node->right);

    return 0;
}


Node* PartialTreeRead(char* buff, int* pos, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, nullptr);
    VERIFICATION(pos == nullptr, "Lost pos address!", logfile, nullptr);

    int offset = 0;
    char command_buff[DEFAULT_SIZE] = {};

    if(buff[*pos] != '('){
        printf("Bad syntax. Remaining buff: %s\n", buff + *pos);
        return nullptr;
    }else{
        (*pos)++;
    }

    Node* node = OpNew("", logfile);

    while(buff[*pos + 1] != '"'){
        (*pos)++;
        node->data[offset++] = buff[*pos];
    }

    node->data[offset + 1] = '\0';

    *pos += 2; // +2 for remaining ' " ' and last symbol in prev string.

    if(buff[*pos] == '('){
        node->left = PartialTreeRead(buff, pos, logfile);
    }else{

        sscanf(buff + *pos, "%3s", command_buff);
        if(!strcmp(command_buff, "nil")){
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
    }else{
        if(buff[*pos] == '('){
        node->right = PartialTreeRead(buff, pos, logfile);
        }
    }

    if(buff[*pos] == ')'){
        (*pos)++;
        return node;
    }

    printf("\nWell...\n");
    return nullptr;
}

int AkinatorGuessing(Root* root, Node* node, FILE* logfile){

    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(node == nullptr, "Input node is nullptr!", logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);

    char ans[DEFAULT_SIZE] = {};

    if(node->left == nullptr && node->right == nullptr){

        printf("Ваш персонаж – %s ?\n", node->data);
        scanf("%s", ans);
        while(!getchar());

        if(!strcmp(ans, "да") || !strcmp(ans, "Да")){
            printf("Отгадал)!\n");
        }else{
            unsigned char depth = 0;

            for(unsigned char x = 0; x < CHAR_BIT; x++){
                if(node->subnet_mask & (1u << x)){
                    depth = x;
                }
            }

            node->right = OpNew(node->data, logfile);
            unsigned char new_ip = node->ip | (unsigned char)(1 << (CHAR_BIT - depth - 1));
            node->right->ip = new_ip;

            unsigned char new_subnet_mask = ~((1u << (CHAR_BIT - depth)) - 1u);
            node->right->subnet_mask = new_subnet_mask;

            printf("Не отгадал( Кто это был?\n");
            scanf("%[^\n]", ans);
            while(!getchar());

            printf("\nХорошо, а чем %s отличается от %s ?\n", ans, node->data);
            scanf("%[^\n]", node->data);
            while(!getchar());

            node->left = OpNew(ans, logfile);
            node->left->ip = node->ip;
            node->left->subnet_mask = new_subnet_mask;

            FILE* databasefile = fopen("akinator_database.txt", "w");

            PrintNode(root->tree_root->right, databasefile);

            fclose(databasefile);
        }
    }else{
        printf("\nВаш персонаж – %s ?\n", node->data);
        bool convergence = false;

        scanf("%s", ans);

        if(!strcmp("Да", ans) || !strcmp("да", ans)){
            AkinatorGuessing(root, node->left, logfile);
            convergence = true;
        }
        if(!strcmp("Нет", ans) || !strcmp("нет", ans)){
            AkinatorGuessing(root, node->right, logfile);
            convergence = true;
        }

        if(!convergence){
            printf("Прости, я не понял твоего ответа.\n");
        }
    }

    return 0;
}

int IpAssignment(Root* root, int initial_depth, FILE* logfile){     // Присваивание адресов определяется след. образом: шаг по
    ECHO(logfile);                                                  // дереву влево обозначается нулевым битом, шаг вправо -
    VERIFICATION_LOGFILE(logfile, -1);                              // битом, выставленным в 1. То есть, пойти влево = 0, пойти
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1); // вправо = 1, по аналогии с расположением чисел 0 и 1
    VERIFICATION(root->tree_root == nullptr, "Input tree root is nullptr!", logfile, -1); // на числовой прямой слева-направо.

    SetMyIp(root->tree_root, initial_depth, logfile);

    return 0;
}

int SetMyIp(Node* node, int depth, FILE* logfile){
    ECHO(logfile);
    VERIFICATION(node == nullptr, "Input node is nullptr", logfile, -1);

    unsigned char new_ip = node->ip | (unsigned char)(1 << (CHAR_BIT - depth - 1));
    unsigned char new_subnet_mask = ~((1 << (CHAR_BIT - depth - 1)) - 1);

    if(depth == 0){
        node->ip = 0;
        node->subnet_mask = 1;
    }

    if(node->left != nullptr){
        node->left->ip = node->ip;
        node->left->subnet_mask = new_subnet_mask;

        SetMyIp(node->left, depth + 1, logfile);
    }

    if(node->right != nullptr){
        node->right->ip = new_ip;
        node->right->subnet_mask = new_subnet_mask;

        SetMyIp(node->right, depth + 1, logfile);
    }

    return 0;
}

int AkinatorDefinition(Root* root, Node* node, char* ans, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(node == nullptr, "Input node is nullptr!", logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    VERIFICATION(ans == nullptr, "Input ans is nullptr!", logfile, -1);

    if(!strcmp(ans, "Неизвестно кто")) {
        printf("Это неизвестно кто.\n");
        return 0;
    }

    unsigned char masked_ip = 0;
    Node finded_node = {};
    OpSearch(node, ans, &finded_node);
    masked_ip = finded_node.ip & finded_node.subnet_mask;

    if(masked_ip == 0){
        printf("Увы, такого персонажа не нашлось...\n");
        fprintf(logfile, "Увы, такого персонажа не нашлось...\n");
        return -1;
    }

    unsigned char subnet_bit = 64;
    printf("Персонаж %s запомнился мне тем, что он(а): ", ans);
    node = root->tree_root->right;

    while(!(node->left == nullptr && node->right == nullptr)){

        if(masked_ip & subnet_bit){
            printf("не ");
            printf("%s", node->data);
            node = node->right;
            printf(", ");
        }else{
            printf("%s", node->data);
            node = node->left;
            printf(", ");
        }

        subnet_bit = subnet_bit >> 1;
    }

    printf("на этом всё.\n");

    return 0;
}

int AkinatorComparing(Root* root, char* first_object, char* second_object, FILE* logfile){
    VERIFICATION_LOGFILE(logfile, -1);
    VERIFICATION(root == nullptr, "Input root is nullptr!", logfile, -1);
    VERIFICATION(first_object == nullptr, "Input first_object is nullptr!", logfile, -1);
    VERIFICATION(second_object == nullptr, "Input second_object is nullptr!", logfile, -1);

    unsigned char first_obj_ip = 0, second_obj_ip = 0;
    Node first_finded_node = {}, second_finded_node = {};
    OpSearch(root->tree_root->right, first_object, &first_finded_node);
    OpSearch(root->tree_root->right, second_object, &second_finded_node);

    if(first_obj_ip == 0){
        printf("Первый персонаж не найден в базе!\n");
        return 0;
    }
    if(second_obj_ip == 0){
        printf("Второй персонаж не найден в базе!\n");
        return 0;
    }

    // for(int i = 0; i < CHAR_BIT; i++){
    //     if("")
    // }
    return 0;
}

unsigned char OpSearch(Node* node, char* correct, Node* ans){
    VERIFICATION(ans == nullptr, "ans is nullptr!", stdout, 1);

    if(!strcmp(node->data, correct)){
        printf("ip персонажа: %d, node->data: %s, subnet_mask: %d, a & b = %d\n",
        node->ip, node->data, node->subnet_mask, node->ip & node->subnet_mask);
        ans->ip = node->ip;
        ans->subnet_mask = node->subnet_mask;
        return 0;                                       // В такой реализации определение можно дать
    }                                                   // только листу, но не узлу. Тк определение в
    else{                                               // исходной задаче нужно было дать персонажу,
        if(node->left != nullptr){                      // решено было остановиться на этом. Определение
            OpSearch(node->left, correct, ans);         // для узла "Неизвестно кто" обрабатывается отдельно.
        }
        if(node->right != nullptr){
            OpSearch(node->right, correct, ans);
        }
    }

    return 0;
}
