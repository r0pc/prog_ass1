#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "uthash.h"
#include "datastructs.h"

extern HashItem* table;
extern HashItem* table1;
extern HashItem* table2;
extern StringStats ss;

void add_item(HashItem** table, const char* key, int value, ItemType itemtype);
int find_item(HashItem* table, const char* key);
void delete_item(HashItem** table, const char* key);
void free_table(HashItem** table);
int increment_item(HashItem* table, const char* key);
void print_table(HashItem* table);
int sort_val_desc(HashItem* a, HashItem* b);
Item* get_top_n(HashItem** table, int n);
void check_substr_inc(HashItem* table, const char* str);
void print_top_n(Item* arr, int n);
int get_total_table(HashItem* table);
void reset_table(HashItem* table);
int get_num_elements(HashItem* table);
int get_num_unique_gtz(HashItem* table);
void num_unique_gtz_grp(HashItem* table);
#endif
