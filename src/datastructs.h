#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include "uthash.h"
#define ALPHABET 256

typedef struct ACNode {
    struct ACNode *next[ALPHABET];
    struct ACNode *fail;
    char *output;    
    int output_f;        
} ACNode;

typedef enum{
    NONE,
    MILD,
    MODERATE,
    SEVERE,
    STOPWORD,
    UNIQUE
}ItemType;

typedef struct {
    char key[256];
    int value;
    ItemType itemtype;
    UT_hash_handle hh;
} HashItem;

typedef struct{
    int val;
    char key[256];
    ItemType itemtype;
}Item;

typedef struct {
    int word_count;
    int total_length;
    int mild_total;
    int severe_total;
    int num_unique;
    int toxic_unique;
    int mild_unique;
    int severe_unique;
    int num_records;
    char filename[256];
} StringStats;


#endif