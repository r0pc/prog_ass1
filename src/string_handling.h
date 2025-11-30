#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datastructs.h"
#include "aho.h"

void normalize_inplace(char* s);
void process_string(ACNode* automation, HashItem* table, char* s);
void print_stringstats();