#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

StringStats ss = { 0 };

void add_item(HashItem** table, const char* key, int value, ItemType itemtype) {
    HashItem* item;
    HASH_FIND_STR(*table, key, item);

    if (!item) {
        item = malloc(sizeof(HashItem));
        strcpy(item->key, key);
        HASH_ADD_STR(*table, key, item);
    }
    item->value = value;
    item->itemtype = itemtype;
}

int find_item(HashItem* table, const char* key) {
    HashItem* item;
    HASH_FIND_STR(table, key, item);
    if (item) return 1;
    return 0;
}

void delete_item(HashItem** table, const char* key) {
    HashItem* item;
    HASH_FIND_STR(*table, key, item);

    if (item) {
        HASH_DEL(*table, item);
        free(item);
    }
}

void free_table(HashItem** table) {
    HashItem* cur, * tmp;
    HASH_ITER(hh, *table, cur, tmp) {
        HASH_DEL(*table, cur);
        free(cur);
    }
}

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

void print_table(HashItem* table) {
    HashItem* item, * tmp;
    HASH_ITER(hh, table, item, tmp) {
        printf("%s -> %d -> %d\n", item->key, item->value, item->itemtype);
    }
}

int get_total_table(HashItem* table) {
    int total = 0;
    HashItem* item, * tmp;
    HASH_ITER(hh, table, item, tmp) {
        total += item->value;
    }
    return total;
}

void reset_table(HashItem* table) {
    HashItem* tmp, * item;
    HASH_ITER(hh, table, item, tmp) {
        item->value = 0;
    }
}

int sort_val_desc(HashItem* a, HashItem* b) {
    return b->value - a->value;
}

char* enum_to_str(ItemType itemtype){
    if(itemtype == SEVERE) return "Severe";
    if(itemtype == MILD) return "Mild";
    if(itemtype == UNIQUE) return "Unique";
    return "err";
}

Item* get_top_n(HashItem** table, int n) {
    if (*table == NULL) {
        printf("table is empty\n");
        return NULL;
    }

    Item* arr = (Item*)malloc(sizeof(Item) * n);
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

// Item *get_top_n_grouped(HashItem **table, int n){
//     if(*table == NULL){
//         printf("table is empty\n");
//         return NULL;
//     }

//     Item *arr = (Item*)malloc(sizeof(Item) * n * 2);
//     HASH_SORT(*table, sort_val_desc);
//     HashItem *item = *table;
//     int i = 0;
//     int c1 = 0;
//     int c2 = n;

//     while(item != NULL || (c1<n && c2<n*2)){
//         if(item->itemtype == SEVERE){
//             strcpy(arr[c1].key, item->key);
//             arr[c1] = 
//         }
//     }

// }

void print_top_n(Item* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d. %s -> %d -> %s\n", i + 1, arr[i].key, arr[i].val, enum_to_str(arr[i].itemtype));
    }
}

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

int get_num_elements(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        c++;
    }
    return c;
}

int get_num_unique_gtz(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        if(item->value > 0){
            c++;
        }
    }
    return c;
}
void num_unique_gtz_grp(HashItem* table) {
    HashItem* item, * tmp;
    int c = 0;
    HASH_ITER(hh, table, item, tmp) {
        if(item->value > 0){
            ss.toxic_unique++;
            if(item->itemtype == SEVERE) ss.severe_unique++;
            if(item->itemtype == MILD) ss.mild_unique++;
        }
    }

}