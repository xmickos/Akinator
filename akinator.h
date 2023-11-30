#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef char* Elem_t;

typedef unsigned char Ip_type;

#define DEFAULT_SIZE 4 * 16
#define DEFAULT_DATA_VALUE 10


#define VERIFICATION_LOGFILE(logfile, retval) if(logfile == nullptr){ printf("\033[1;31mError\033[0m: logfile is nullptr!\nExiting...\n"); return retval;}

#define ECHO(logfile) fprintf(logfile, "[%s, %d]\n", __FUNCTION__, __LINE__);                                          \
    printf("[%s, %d]\n", __FUNCTION__, __LINE__);

#define VERIFICATION(condition, message, logfile, ret_val)     if(condition){                                          \
        fprintf(logfile, "[%s, %d]Error:\t" message "\nExiting...\n", __FUNCTION__, __LINE__);                         \
        return ret_val;                                                                                                \
    }

#define ERROR_MSG(msg) printf("\033[1;31mError\033[0m: " msg)

#define GRAPHVIZ_INIT "digraph g {\nfontname=\"Helvetica,Arial,sans-serif\"\n\
node [fontname=\"Helvetica,Arial,sans-serif\"]\n\
edge [fontname=\"Helvetica,Arial,sans-serif\"]\n\
\ngraph [\n\trankdir = \"LR\"\n];\n\n\
node [\n\tfontsize = \"16\"\n\tshape = \"ellipse\"\n];\n\n\
edge [ ];\n\n"

#define GRAPHVIZ_MKNODE(name, data) #name "[\n\tlabel = \"<f0> data: " data "| {<f1> Left| <f2> Right} | <f3> " name "\"\n\tshape = \"record\"\n];\n\n"

#define GRAPHVIZ_CONNECT_NODE(first_id, first_domain, second_id, second_domain) first_id ":" #first_domain " -> " second_id ":" #second_domain "\n\n"

#define GRAPHVIZ_END "}\n"

struct Node{
    Node* left  = nullptr;
    Elem_t data = nullptr;
    Node* right = nullptr;
    Ip_type ip = 0;
    unsigned char subnet_mask = 0;
};

struct Root{
    size_t size = 0;
    Node* tree_root = nullptr;
};

Node* OpNew(Elem_t value, FILE* logfile);

int RootCtor(Root* root, FILE* logfile);

int OpInsertSort(Root* root, Elem_t value, FILE* logfile);

int TreePartialTextDump(Node* node, size_t indent, FILE* logfile);

int OpPartialGraphDump(Node* node, unsigned char ip, unsigned char depth, FILE* dotfile, FILE* logfile);

int OpGraphDump(Root* root, FILE* dotfile, FILE* logfile);

void IndentMe(size_t count, FILE* logfile);

void PrintNode(const Node* node, FILE* logfile);

int ReadTree(FILE* backupfile, Node* init_node, FILE* logfile);

Node* PartialTreeRead(char* buff, int* pos, FILE* logfile);
