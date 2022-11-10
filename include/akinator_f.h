#ifndef AKINATOR_F_H
#define AKINATOR_F_H

#include "text_funcs.h"

struct Node
{

    const char *data;
    Node * l_son;
    Node * r_son;
    Node * parent;

};

struct Tree_t
{
    Node * main_node;
    FILE * data_base;
};

enum openModes
{
    GUESS_OBJECT                = 0,
    GET_DEFINITION_OF_AN_OBJECT = 1,
    COMPARE_TWO_OBJECTS         = 2,
}; 

enum son
{
    LEFT_SON  = 0,
    RIGHT_SON = 1,
}; 

Node * nodeCtor();
Node * nodeConnect(Node *parent, const char dest);
Node * findNode(Node *node, const char *string);

int runThrough(const Node* start_node);
int nodeDtor(Node *node);

void printPre(const Node * node);
void printIn(const Node * node);
void printPost(const Node * node);

int openBaseToRewrite();
void printPreFile(const Node * node);
int printTree(Tree_t * tree);

int readDataBase(Text_info *text, size_t line_idx, int free_port, Node * node);
int treeCtor(Tree_t *tree, const char mode);
int treeDtor(Tree_t *tree);

#endif