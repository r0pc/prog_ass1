#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toxic_loading.h"
#include "hashtable.h"

void load_toxic(HashItem** table, const char* filename){
    FILE *fptr = fopen(filename, "r");
    if(!fptr){
        printf("file [%s] not found", filename);
        return;
    }
    ItemType itemtype = get_itemtype(filename);
    
    char line[256];
    while(fgets(line, sizeof(line), fptr)){
        line[strcspn(line, "\r\n")] = '\0';

        if(line[0] == '\0') continue;

        add_item(table, line, 0, itemtype);
    }
}

ItemType get_itemtype(const char* filename){
    ItemType itemtype = NONE;
    if(strcmp(filename, "data/mild_words.txt") == 0){
        itemtype = MILD;
    } else if (strcmp(filename, "data/moderate.txt") == 0){
        itemtype = MODERATE;
    } else if (strcmp(filename, "data/severe_words.txt") == 0){
        itemtype = SEVERE;
    } else if(strcmp(filename, "data/stopword.txt") == 0){
        itemtype = STOPWORD;
    }

    return itemtype;
}
