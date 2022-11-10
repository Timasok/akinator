#include <string.h>
#include "akinator_modes.h"
#include "akinator_debug.h"

// sstatic int nodeNumber = 0;
static bool guessed = false;
static const Node * guessedNode = nullptr;

static int printAnswerInfo()
{
    printf("Если ДА нажмите (y) если НЕТ нажмите (n)\n");
    // printf("if YES press (y), if NO press (n)\n");
    return 0;
}

static int getAnswer(const Node * node, char *answer)
{

    *answer = ' ';
    printAnswerInfo();
    scanf(" %c", answer); 
    
    if (*answer == 'y' || *answer == 'n')
    {
        return 0;
    }else{
        printf("Ошибка ввода повторите\n");
        getAnswer(node, answer);     
    }
    
}

static int printQuestion(const char * string)
{
    printf("\e[0;31m%s\e[0m?\n", string);
    return 0;
}

static int processGuessed(Node *node)
{
    // if (guessed == false)
    // {
    //     guessed = true;
        guessedNode = node;
        printf("Is it ");
        printQuestion(node->data);
        
        char answer = ' ';
        getAnswer(node, &answer);
        if(answer == 'n')
        {
            addNode(node);            
        }else
        {
            printf("I guessed it ha!");
        }

    // }

    return 0;
}

int addNode(Node * node)
{
    if (node->l_son != nullptr || node->r_son != nullptr)
        return AK_ERROR_ADDING_NODE_TO_PARENT;

    char correct_object[128]      = {};
    char clarifying_question[128] = {};

    printf("Введите правильный объект\n");
    scanf("%s", correct_object);

    printf("Чем %s отличается от %s?\n", correct_object, node->data);
    scanf("%s", clarifying_question);
    
    Node * incorrect_value = nodeConnect(node, RIGHT_SON);
    Node * correct_value = nodeConnect(node, LEFT_SON);

    incorrect_value->data = strdup(node->data);
    correct_value->data = strdup(correct_object);
    node->data = strdup(clarifying_question);

    return 0;

}

static int getAnswerNext(Node* node)
{
    if (!node)
        return 0;

    char answer = ' ';
    nodeDump(node);   

    if (node->l_son == nullptr && node->r_son == nullptr)
    {
        processGuessed(node);
        return 0;
    }    
    
    printQuestion(node->data);
    getAnswer(node, &answer);

    switch(answer)
    {
        case 'y':
            getAnswerNext(node->l_son);
            break;
        case 'n':
            getAnswerNext(node->r_son);
            break;
        default:
            break;
    }

    return 0;

}

int guessObject(Tree_t *tree){

    getAnswerNext(tree->main_node->l_son);
    return 0;
}

int defineObject(Tree_t *tree, const char *object_data);
int compareObjects(Tree_t *tree, const char *object1_data, const char *object2_data);