#ifndef AHO_H
#define AHO_H

#include "datastructs.h"
#include "string_handling.h"

ACNode* ac_create_node();
ACNode* ac_build(HashItem *table);
void ac_search(ACNode *root, const char *text, HashItem *table);
void ac_free(ACNode* root);

#endif
