#include <stdlib.h>
#include <stdio.h>

typedef int Elem_t;

#define DEFAULT_SIZE 16
#define DEFAULT_DATA_VALUE 10

#define VERIFICATION_LOGFILE(logfile) {}

#define ECHO(logfile) fprintf(logfile, "[%s, %d]\n", __FUNCTION__, __LINE__);                                        \
    printf("[%s, %d]\n", __FUNCTION__, __LINE__);

#define VERIFICATION(condition, message, logfile, ret_val)     if(condition){                                        \
        fprintf(logfile, "[%s, %d]Error:\t" message "\nExiting...\n", __FUNCTION__, __LINE__);                       \
        return ret_val;                                                                                              \
    }

#define ERROR_MSG(msg) {}

struct Node{
    Node* left  = nullptr;
    Elem_t data = 0;
    Node* right = nullptr;
};

struct Root{
    size_t size = 0;
    Node* tree_root = nullptr;
};

Node* OpNew();

Root RootCtor(FILE* logfile);

int OpInsertSort(Root* root, Elem_t value, FILE* logfile);

int TreePartialTextDump(Node* node, size_t indent, FILE* logfile);
