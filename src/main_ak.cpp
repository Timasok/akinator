#include <stdio.h>

#include "text_funcs.h"
#include "akinator_f.h"
#include "akinator_debug.h"

int main()
{

    Text_info txt = {};

    textCtor(&txt, "data_base.txt");
    printText(&txt);

    Tree_t first = {};
    treeCtor(&first, GUESS_OBJECT);

    first.main_node = nodeCtor();
    Node * is_wild = nodeConnect(first.main_node, LEFT_SON);
    Node * dont_exist = nodeConnect(first.main_node, RIGHT_SON);
    Node * teacher = nodeConnect(is_wild, LEFT_SON);
    Node * neighbour = nodeConnect(is_wild, RIGHT_SON);
    
    is_wild->data         = "is_wild";
    dont_exist->data      = "dont_exist";
    teacher->data         = "teacher";
    neighbour->data       = "neighbour";
    first.main_node->data = "animal";

    // is_wild->data         = "5";
    // dont_exist->data      = "15";
    // teacher->data         = "3";
    // neighbour->data       = "6";
    // first.main_node->data = "10";

    printTree(&first);

    TREE_DUMP(&first);

    textDtor(&txt);
    nodeDtor(first.main_node);
    treeDtor(&first);

    return 0;
}
    // runThrough(first.main_node);