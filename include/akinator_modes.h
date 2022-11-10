#ifndef AK_MODES_H
#define AK_MODES_H

#include "akinator_f.h"

int addNode(Node * node);

int guessObject(Tree_t *tree);
int defineObject(Tree_t *tree, const char *object_data);
int compareObjects(Tree_t *tree, const char *object1_data, const char *object2_data);

#endif 