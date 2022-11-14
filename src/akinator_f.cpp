#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "akinator_modes.h"
#include "akinator_debug.h"

static FILE *base = nullptr;
static int nodeNumber = 0;
static char indent[64] = "\n";

int sayAndWrite(const char * sentence)
{   
    char command[MAX_BUFFER_LENGTH] = {};
    // sprintf(command, "festival -b \'(SayText \"%s\")\'", sentence);
    sprintf(command, "echo \"%s\" | festival --tts --language russian", sentence);
    // sprintf(command, "echo \"%s\" | festival --tts --language russian");
    printf("%s", sentence);
    system(command);
    return 0;
}

int treeCtor(Tree_t *tree, const char mode)
{
    nodeCtor(&tree->main_node);

    switch (mode)
    {
        case GUESS_OBJECT:
            tree->data_base = fopen("database.txt", "r+");        
            break;

        case DEFINE_OBJECT:
            tree->data_base = fopen("database.txt", "r");        
            break;

        case COMPARE_TWO_OBJECTS:
            tree->data_base = fopen("database.txt", "r");        
            break;
    
        default:
            return AK_ERROR_WRONG_MODE;
            break;
    }

    return 0;
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

static int openBaseToRewrite()
{
    base = fopen("database.txt", "w");
    return 0;
}

int readDataBase(Text_info *text, size_t line_idx, int free_port, Node * node)
{
    if (line_idx < 0 || line_idx >= text->number_of_lines)
        return AK_ERROR_SIZE_T_OVERFLOW;

    Node * new_node = nullptr;

    char data[MAX_BUFFER_LENGTH] = {};
    const char * fig_start  = strchr(text->lines[line_idx], '{');
    const char * fig_finish = strchr(text->lines[line_idx], '}');

    // printf("\nfig_start = %s fig_finish = %s", fig_start, fig_finish);
    if (fig_start == nullptr && fig_finish == nullptr)
    {
        readDataBase(text, line_idx + 1, free_port, node);
        return 0;
    }

    if (fig_start == nullptr && fig_finish != nullptr)    
    {
        readDataBase(text, line_idx + 1, RIGHT_SON, node->parent);
        return 0;
    }

    if (fig_start != nullptr)
    {
        // sscanf(fig_start + 2, "%s", data);
        sscanf(fig_start + 3, "%[^\n]s", data);

        if (strchr(data, '"') != nullptr);
        {
            int data_len = strchr(data, '"') - data;
            data[data_len] = '\0';
        }

        if (fig_finish != nullptr)
        {
            new_node = nodeConnect(node, free_port);
            new_node->data = strdup(data);
            // printf("old port = %s new node = %s port = %d", node->data, new_node->data, free_port);   
            readDataBase(text, line_idx + 1, (free_port + 1) % 2, node);

        }else
        {

            new_node = nodeConnect(node, free_port);
            new_node->data = strdup(data);
            // printf("old port = %s new node = %s port = %d", node->data, new_node->data, free_port);   
            readDataBase(text, line_idx + 1, LEFT_SON, new_node);
        }
    
        
        return 0;
    }    
    
    return 1;

}

int saveBase(const Node *start_node)
{
    openBaseToRewrite();
    printPreFile(start_node);
    fprintf(base, "}");
    fclose(base);
    return 0;
}

//tree_funcs_extract

int treeDtor(Tree_t *tree)
{
    // fclose(tree->data_base);  //todo закрывай только нужные дескрипторы
    nodeDtor(tree->main_node);
    free(tree->main_node);

    return 0;
}

int nodeCtor(Node **node)
{
    *node = (Node *)calloc(1, sizeof(Node));
    return 0;
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

    new_node->parent = parent;
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

Node * findNode(Node *node, const char *string, Stack * answers)
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

        if (result != nullptr)
            stackPush(answers, YES);
    }

    if (node->r_son && result == nullptr)
    {
        result = findNode(node->r_son, string);

        if (result != nullptr)
            stackPush(answers, NO);
    }

    return result;

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
    // fprintf(base, "{ %s", node->data);

    fprintf(base, "{ \"%s\"", node->data);
    strcat(indent, "\t");

    if (node->l_son)
    {
        fprintf(base, "%s", indent);
        printPreFile(node->l_son);
        fprintf(base, "}");

        // todo можно с помощью принфа просто вывести строку заполненную
        // одним определенным символом
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

int runThrough(const Node* start_node)
{
    operNext(start_node, *oper);
    return 0;
}

int nodeDtor(Node *node)
{
    if (!node)
        return 0;

    while(1)
    {
        
        // printf("\e[0;32mдолжны были зафришеть - \e[0m\n");
        // nodeDump(node);
        if (node->l_son == nullptr && node->r_son == nullptr)
        {
            if (node->parent == nullptr)
                return 0;

            if (node->parent->l_son == node)
            {
                node->parent->l_son = nullptr;
                    
            }
            else if(node->parent->r_son == node)
            {
                node->parent->r_son = nullptr;
            }

            // printf("\e[0;31mзафришен - \e[0m\n");
            // nodeDump(node);
            free((void *)node->data);
            free(node);
            return 0;

        }

        nodeDtor(node->l_son);

        nodeDtor(node->r_son);
    }

    return 0;
}