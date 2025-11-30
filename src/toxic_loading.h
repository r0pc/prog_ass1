#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datastructs.h"
#include "hashtable.h"

void load_toxic(HashItem** table, const char* filename);
ItemType get_itemtype(const char* filename);