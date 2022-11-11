#include <string.h>
#include "akinator_modes.h"
#include "akinator_debug.h"

// sstatic int nodeNumber = 0;
static bool guessed = false;
static const Node * guessedNode = nullptr;

//TODO add to text func замещение конца

static int printAnswerInfo()
{
    printf("[y\\n]\n");
    return 0;
}

static int getAnswer(const Node * node, char *answer)
{

    printAnswerInfo();
   
    int symbol = getchar(); 
    
    if (symbol != '\n')
        ungetc(symbol, stdin);
      
    *answer = getchar();

    while (getchar() != '\n')
        ;

    if (*answer == 'y' || *answer == 'n')
    {
        return 0;
    }else{
        printf("Ошибка ввода повторите\n");
        getAnswer(node, answer);
    }

    return 0;    
}

static int printQuestion(const char * string)
{
    printf("\e[0;32m%s\e[0m?", string);
    return 0;
}

static int processGuessed(Node *node)
{
    guessedNode = node;
    printf("Это ");
    printQuestion(node->data);
    
    char answer = ' ';
    getAnswer(node, &answer);
    if(answer == 'n')
    {
        addNode(node);
        printf("Добавил в базу! Больше с этим не проведёте\n");         
    }else
    {
        printf("Эхху! Я угадал блэт!\n");
    }

    return 0;
}



int addNode(Node * node)
{
    if (node->l_son != nullptr || node->r_son != nullptr)
        return AK_ERROR_ADDING_NODE_TO_PARENT;

    char correct_object[MAX_BUFFER_LENGTH]      = {};
    char clarifying_question[MAX_BUFFER_LENGTH] = {};
    int len = 0;

    fprintf(stdout, "Введите правильный объект\n");

    fgets(correct_object, MAX_BUFFER_LENGTH, stdin);
    strchr(correct_object, '\n')[0] = '\0';
    // printf("reading %d\n", correct_object[0]);

    fprintf(stdout, "Чем %s отличается от %s\n", correct_object, node->data);
    fgets(clarifying_question, MAX_BUFFER_LENGTH, stdin);
    strchr(clarifying_question, '\n')[0] = '\0';

    // fprintf(stdout, "correct object %s clarifying question %s\n",correct_object, clarifying_question);

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
    // nodeDump(node);   

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