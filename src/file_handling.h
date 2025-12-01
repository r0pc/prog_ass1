#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashtable.h"
#include "aho.h"
#include "string_handling.h"

FILE* get_file(int* filetype);
int check_filetype(char* filename);
void read_txt(FILE* fptr, ACNode* automation, HashItem* table);
void read_csv(FILE* fptr, ACNode* automation, HashItem* table);
char* csv_get_col(char* row, int target_col);
void process_file(ACNode* automation, HashItem* table);