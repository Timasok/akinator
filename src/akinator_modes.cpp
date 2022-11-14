#include <string.h>
#include "akinator_modes.h"
#include "akinator_debug.h"

// sstatic int nodeNumber = 0;
static bool guessed = false;
static const Node * guessedNode = nullptr;
static bool proceed_definition = false;

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
        printf("Эхху! Я угадал блэт! Давай ещё!!!\n");
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

static int getPreviousPhrase(Node* node, Node* primal_node)
{
    if (!node || node == primal_node)
        return 0;

    // printf("\e[0;31m%s\e[0m", node->data);

    char line_to_say[MAX_BUFFER_LENGTH] = {};

    if (node->l_son == nullptr && node->r_son == nullptr)
    {
        sprintf(line_to_say, "%s - это", node->data);
       
    }else
    {
        sprintf(line_to_say, " %s", node->data);

    }
        SAY_AND_WRITE(line_to_say); 

    if (node->parent->l_son == node)
    {
        getPreviousPhrase(node->parent, primal_node);

    }

    if (node->parent->r_son == node)
    {
        SAY_AND_WRITE(" не"); 
        getPreviousPhrase(node->parent, primal_node);

    }

    return 0;

}

//TODO strange 1 function to do one function
int guessObject(Tree_t *tree){

    getAnswerNext(tree->main_node->l_son);
    return 0;
}

int defineObject(Tree_t *tree)
{
    char object_data[MAX_BUFFER_LENGTH] = {};
    SAY_AND_WRITE("Введите объект который хотите найти в базе\n");
    scanf( "%*[^\n]" );
    scanf("%s", &object_data);                        
//     if (object_data[0] == 0)
//         break;

    Node *correct_node = findNode(tree->main_node->l_son, object_data);

    proceed_definition = true;

    if (correct_node == nullptr)
    {
        char line_to_say[MAX_BUFFER_LENGTH] = {};
        sprintf(line_to_say, "Объект %s не найден в базе. Сделайте другой запрос.\n", object_data);
        SAY_AND_WRITE(line_to_say);
    
    }else
    {
        getPreviousPhrase(correct_node, tree->main_node);
        SAY_AND_WRITE("\n");
    }

    proceed_definition = false;

    return 0;
}


int compareObjects(Tree_t *tree)
{
    // char first_object[MAX_BUFFER_LENGTH]  = {};
    // char second_object[MAX_BUFFER_LENGTH] = {};
    // char line_to_say[MAX_BUFFER_LENGTH] = {};

    // SAY_AND_WRITE("Введите первый объект\n");
    // scanf( "%*[^\n]" );
    // scanf("%s", first_object);      
    
    // SAY_AND_WRITE("Введите второй объект\n");
    // scanf( "%*[^\n]" );
    // scanf("%s", second_object); 

    // sprintf(line_to_say, "%s %s", first_object, second_object);
    // SAY_AND_WRITE(line_to_say);

    // Node *first_node = findNode(tree->main_node->l_son, first_object);
    // Node *second_node = findNode(tree->main_node->l_son, second_object);

    // first_node->data = first_object;


    return 0;
}