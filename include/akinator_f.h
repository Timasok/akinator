#ifndef AKINATOR_F_H
#define AKINATOR_F_H

#define DBG_OUT fprintf(stderr, "file: %s func: %s line: %d\n", __FILE__, __FUNCTION__, __LINE__)

// #define sayAndWrite(sentence)  printf("%s", sentence);        \
                    
#include "text_funcs.h"
#include "stack_funcs.h"

struct Node
{

    const char *data;
    Node * l_son = nullptr;
    Node * r_son = nullptr;
    Node * parent = nullptr;

};

struct Tree_t
{
    Node * main_node;
    FILE * data_base;
};

enum son
{
    LEFT_SON  = 0,
    RIGHT_SON = 1,
};

enum answer
{
    YES  = 0,
    NO   = 1,
}; 

const int MAX_BUFFER_LENGTH = 512;

int nodeCtor(Node **node);
Node * nodeConnect(Node *parent, const char dest);
int sayAndWrite(const char * sentence);

Node * findNode(Node *node, const char *string, Stack * answers);
Node * findNode(Node *node, const char *string);

int nodeAssert(Node * node);

int runThrough(const Node* start_node);
int nodeDtor(Node *node);

void printPre(const Node * node);
void printIn(const Node * node);
void printPost(const Node * node);

int saveBase(const Node *start_node);
void printPreFile(const Node * node);
int printTree(Tree_t * tree);

int readDataBase(Text_info *text, size_t line_idx, int free_port, Node * node);
int treeCtor(Tree_t *tree, const char mode);
int treeDtor(Tree_t *tree);

#endif