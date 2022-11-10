#include <stdio.h>

#include "text_funcs.h"
#include "akinator_modes.h"
#include "akinator_debug.h"

int main()
{

    Tree_t second = {};

    Text_info txt = {};
    textCtor(&txt, "database.txt");
    printText(&txt);
    
    second.main_node = nodeCtor();
    readDataBase(&txt, 0, LEFT_SON, second.main_node);
    textDtor(&txt);  

    TREE_DUMP(&second);

    guessObject(&second);

    TREE_DUMP(&second);

    // // openBaseToRewrite();
    // printPreFile(second.main_node);

    treeDtor(&second);
    
    return 0;
}
    // runThrough(first.main_node);