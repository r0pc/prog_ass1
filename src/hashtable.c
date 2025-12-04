#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"


// https://troydhanson.github.io/uthash/userguide.html
// hashtable implementation using uthash library

StringStats ss = { 0 };

// adds item if item doesnot exist in hashtable
void add_item(HashItem** table, const char* key, int value, ItemType itemtype) {
    HashItem* item;
    HASH_FIND_STR(*table, key, item);

    if (!item) {
        item = malloc(sizeof(HashItem));
        if(!item){
            printf("Malloc failed while creating item for hashmap\n");
            printf("Aborting Program\n");
            exit(EXIT_FAILURE);
        }
        strcpy(item->key, key);
        HASH_ADD_STR(*table, key, item);
    }
    item->value = value;
    item->itemtype = itemtype;
}

// finds a item in the table
int find_item(HashItem* table, const char* key) {
    HashItem* item;
    HASH_FIND_STR(table, key, item);
    if (item) return 1;
    return 0;
}

// deletes an item in the table
void delete_item(HashItem** table, const char* key) {
    HashItem* item;
    HASH_FIND_STR(*table, key, item);

    if (item) {
        HASH_DEL(*table, item);
        free(item);
    }
}

// frees all items in the table
void free_table(HashItem** table) {
    HashItem* cur, * tmp;
    HASH_ITER(hh, *table, cur, tmp) {
        HASH_DEL(*table, cur);
        free(cur);
    }
}

// checks if item exists in the table, if it does inc its val
int increment_item(HashItem* table, const char* key) {
    HashItem* item;
    HASH_FIND_STR(table, key, item);
    if (item) {
        item->value++;
        if (item->itemtype == SEVERE) {
            ss.severe_total++;
        }
        if (item->itemtype == MILD) {
            ss.mild_total++;
        }
        return 1;
    }
    return 0;
}

// prints the table for debugging
void print_table(HashItem* table) {
    HashItem* item, * tmp;
    HASH_ITER(hh, table, item, tmp) {
        printf("%s -> %d -> %d\n", item->key, item->value, item->itemtype);
    }
}

// gets total of the table
int get_total_table(HashItem* table) {
    int total = 0;
    HashItem* item, * tmp;
    HASH_ITER(hh, table, item, tmp) {
        total += item->value;
    }
    return total;
}

// resets all vals in the table to 0
void reset_table(HashItem* table) {
    HashItem* tmp, * item;
    HASH_ITER(hh, table, item, tmp) {
        item->value = 0;
    }
}

// used by uthash mergesort algorithm to sort in desc order
int sort_val_desc(HashItem* a, HashItem* b) {
    return b->value - a->value;
}

// returns string for respective enum val
char* enum_to_str(ItemType itemtype) {
    if (itemtype == SEVERE) return "Severe";
    if (itemtype == MILD) return "Mild";
    if (itemtype == UNIQUE) return "Unique";
    return "err";
}

// asks user for input n, returns an arr of size n of top items
Item* get_top_n(HashItem** table, int n) {
    if (*table == NULL) {
        printf("table is empty\n");
        return NULL;
    }

    Item* arr = (Item*)malloc(sizeof(Item) * n);
    if(!arr){
        printf("Malloc failed while building top_n item arr\n");
        printf("Aborting program...\n");
        exit(EXIT_FAILURE);
    }
    HASH_SORT(*table, sort_val_desc);
    HashItem* item = *table;
    int i = 0;
    int c = 0;
    while (item != NULL && c < n) {
        strcpy(arr[i].key, item->key);
        arr[i].val = item->value;
        arr[i].itemtype = item->itemtype;
        item = item->hh.next;
        i++;
        c++;
    }
    return arr;
}

// prints top n
void print_top_n(Item* arr, int n) {
    char bar[] = "##################################################";
    int highest = arr[0].val;
    int bar_len = strlen(bar);
    printf("%-4s| %-15s | %-7s| %-8s | %s\n", "Rank", "Word", "Count", "Severity", "Graph");

    printf("----|-----------------|--------|----------|--------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-15s | %6d | %8s | ", i + 1, arr[i].key, arr[i].val, enum_to_str(arr[i].itemtype));
        int range = (int)((long)arr[i].val * bar_len / highest);
        for (int j = 0; j < range; j++) {
            printf("%c", bar[j]);
        }
        printf("\n");
    }
}

void fprintf_top_n(Item* arr, int n, FILE *fptr){
    char bar[] = "##################################################";
    int highest = arr[0].val;
    int bar_len = strlen(bar);
    fprintf(fptr,"%-4s| %-15s | %-7s| %-8s | %s\n", "Rank", "Word", "Count", "Severity", "Graph");

    fprintf(fptr,"----|-----------------|--------|----------|--------------------\n");
    for (int i = 0; i < n; i++) {
        fprintf(fptr,"%-3d | %-15s | %6d | %8s | ", i + 1, arr[i].key, arr[i].val, enum_to_str(arr[i].itemtype));
        int range = (int)((long)arr[i].val * bar_len / highest);
        for (int j = 0; j < range; j++) {
            fprintf(fptr,"%c", bar[j]);
        }
        fprintf(fptr,"\n");
    }
}


// old substring checking function O(n^2) -- slow
void check_substr_inc(HashItem* table, const char* str) {
    HashItem* item, * tmp;
    size_t len = strlen(str);
    HASH_ITER(hh, table, item, tmp) {
        size_t keylen = strlen(item->key);

        if (keylen > len) continue;

        for (size_t i = 0; i <= len - keylen; i++) {
            if (strncmp(str + i, item->key, keylen) == 0) {
                item->value++;
            }
        }
    }
}


// gets number of elements contained in a table
int get_num_elements(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        c++;
    }
    return c;
}

// gets number of elements in a table with val > 0
int get_num_unique_gtz(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        if (item->value > 0) {
            c++;
        }
    }
    return c;
}

// gets number of elements in a table with val > 0 according to enum type, for toxic words table
void num_unique_gtz_grp(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        if (item->value > 0) {
            ss.toxic_unique++;
            if (item->itemtype == SEVERE) ss.severe_unique++;
            if (item->itemtype == MILD) ss.mild_unique++;
        }
    }

}