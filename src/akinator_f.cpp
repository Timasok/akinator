#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "akinator_f.h"
#include "akinator_debug.h"

static FILE *base = nullptr;
static int nodeNumber = 0;
static char indent[64] = "\n";

int treeCtor(Tree_t *tree, const char mode)
{
    tree->main_node = nodeCtor();

    switch (mode)
    {
        case GUESS_OBJECT:
            tree->data_base = fopen("database.txt", "r+");        
            break;

        case GET_DEFINITION_OF_AN_OBJECT:
            tree->data_base = fopen("database.txt", "r");        
            break;

        case COMPARE_TWO_OBJECTS:
            tree->data_base = fopen("database.txt", "r");        
            break;
    
        default:
            return AK_ERROR_WRONG_MODE;
            break;
    }

    base = fopen("database.txt", "w+");

    return 0;
}

int treeDtor(Tree_t *tree)
{
    fcloseall();

    return 0;
}

Node * nodeCtor()
{
    Node * new_node = (Node *)calloc(1, sizeof(Node));
    
    return new_node;
}

Node * nodeConnect(Node *parent, const char dest)
{
    assert(parent != NULL);
    
    Node * new_node = (Node *)calloc(1, sizeof(Node));
    
    switch(dest)
    {
        case LEFT_SON:
                parent->l_son = new_node;
                break;
        case RIGHT_SON:
                parent->r_son = new_node;
                break;
        default:
                //letim
                break;
    }

    return new_node;
}

int printTree(Tree_t * tree)
{
    printf("PrintTree\n");
    
    printf("PRE_ORDER:\n\t");
    printPre(tree->main_node);
    printf("}\n");

    printf("IN_ORDER:\n\t");
    printIn(tree->main_node);
    printf("}\n");

    printf("POST_ORDER:\n\t");
    printPost(tree->main_node);
    printf("}\n");

    printPreFile(tree->main_node);
    fprintf(base, "}\n");

    return 0;

}

void printPre(const Node * node)
{
    if (!node) 
        return;

    printf("{ ");
    printf("%s ", node->data);

    if (node->l_son)
    {
        printPre(node->l_son);
        printf("} ");
    }

    if (node->r_son)
    {
        printPre(node->r_son);
        printf("} "); 
    }

    return;
}

void printIn(const Node * node)
{
    if (!node) 
        return;

    printf("{ ");

    if (node->l_son)
    {
        printIn(node->l_son);
        printf("} ");
    }

    printf("%s ", node->data);

    if (node->r_son)
    {
        printIn(node->r_son);
        printf("} "); 
    }

    return;
}

void printPost(const Node * node)
{
    if (!node) 
        return;

    printf("{ ");

    if (node->l_son)
    {
        printPost(node->l_son);
        printf("} ");
    }

    if (node->r_son)
    {
        printPost(node->r_son);
        printf("} "); 
    }

    printf("%s ", node->data);

    return;
}

Node * findNode(Node *node, const char *string)
{

    Node *result = nullptr;

    if (!node) 
        return result;

    if (strcmp(string, node->data) == 0)
    {
        result = node;
        return result;
    }

    if (node->l_son)
    {
        result = findNode(node->l_son, string);
    }

    if (node->r_son && result == nullptr)
    {
        result = findNode(node->r_son, string);
    }

    return result;

}

void printPreFile(const Node * node)
{
    if (!node)
        return;
        
    int len = 0;
    fprintf(base, "{ %s", node->data);
    strcat(indent, "\t");

    if (node->l_son)
    {
        fprintf(base, "%s", indent);
        printPreFile(node->l_son);
        fprintf(base, "}");

        len = strlen(indent);
        if (len >= 2) 
            indent[len-1] = '\0'; 

    }

    if (node->r_son)
    {
        fprintf(base, "%s", indent);
        printPreFile(node->r_son);
        fprintf(base, "}"); 

        len = strlen(indent);
        if (len >= 2)
            indent[len-1] = '\0';

    }

    if (node->l_son || node->r_son)
    {
        len = strlen(indent);
        if (len >= 2)
            indent[len-1] = '\0';
        fprintf(base, "%s", indent);
        strcat(indent, "\t");

    }

    return;
}

static int oper(const Node * node)
{
    printf("Node data %s number %d\n", node->data, nodeNumber);
    return 0;
}

static int operNext(const Node* node, int (*oper)(const Node *))
{
    if (!node)
        return 0;

    nodeNumber++;    
    oper(node);

    if (node->l_son)
    {
        operNext(node->l_son, *oper);

    }

    if (node->r_son)
    {
        operNext(node->r_son, *oper);

    }

    return 0;

}

int runThrough(const Node* start_node)
{
    operNext(start_node, *oper);
    return 0;
}


int nodeDtor(Node *node)
{

    if (node->l_son != NULL)
    {
        nodeDtor(node->l_son);
        node->l_son = NULL;

    }else if(node->r_son != NULL)
    {
        nodeDtor(node->r_son);
        node->r_son = NULL;
    }

    free(node);   
    node = NULL;
    return 0;
}