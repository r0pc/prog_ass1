#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include "uthash.h"
#define ALPHABET 256

// this file contains all the structs definations used


// ACNode used in Aho crosaick algo, substring searching
// ALPHABET is 256, to capture all ascii characters
// next ptr arr to create trie datastruct
// fail ptr used to create fail links for algo
// int output_f is used as a boolean to check whether this node has a value on it when freeing
// output is the string used as key in hashmap

typedef struct ACNode {
    struct ACNode* next[ALPHABET];
    struct ACNode* fail;
    char* output;
    int output_f;
} ACNode;

typedef enum {
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

typedef struct {
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