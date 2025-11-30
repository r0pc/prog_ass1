#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aho.h"
#include "hashtable.h"

ACNode* ac_create_node() {
    ACNode* node = calloc(1, sizeof(ACNode));
    return node;
}

static void ac_insert(ACNode* root, const char* word) {
    ACNode* p = root;
    for (const unsigned char* c = (unsigned char*)word; *c; c++) {
        if (!p->next[*c])
            p->next[*c] = ac_create_node();
        p = p->next[*c];
    }
    p->output = strdup(word);
    p->output_f = 1;
}

// black magic
static void ac_build_fail_links(ACNode* root) {
    ACNode** queue = malloc(200000 * sizeof(ACNode*));
    int head = 0, tail = 0;

    root->fail = root;
    queue[tail++] = root;

    while (head < tail) {
        ACNode* r = queue[head++];

        for (int c = 0; c < ALPHABET; c++) {
            ACNode* u = r->next[c];
            if (!u) continue;

            ACNode* v = r->fail;
            while (v != root && !v->next[c])
                v = v->fail;

            if (v->next[c] && v->next[c] != u)
                v = v->next[c];

            u->fail = v;

            if (u->fail->output && !u->output)
                u->output = u->fail->output;

            queue[tail++] = u;
        }
    }

    free(queue);
}

ACNode* ac_build(HashItem* table) {
    ACNode* root = ac_create_node();
    HashItem* it, * tmp;

    HASH_ITER(hh, table, it, tmp) {
        ac_insert(root, it->key);
    }
    ac_build_fail_links(root);

    return root;
}

void ac_search(ACNode* root, const char* text, HashItem* table) {
    ACNode* p = root;

    for (const unsigned char* c = (unsigned char*)text; *c; c++) {

        while (p != root && !p->next[*c])
            p = p->fail;

        if (p->next[*c])
            p = p->next[*c];

        if (p->output)
            increment_item(table, p->output);
    }
}

void ac_free(ACNode* root) {
    if (!root) return;

    ACNode** queue = malloc(200000 * sizeof(ACNode*));
    int head = 0, tail = 0;

    queue[tail++] = root;

    while (head < tail) {
        ACNode* node = queue[head++];


        for (int c = 0; c < ALPHABET; c++) {
            if (node->next[c]) {
                queue[tail++] = node->next[c];
            }
        }

        if (node->output && node->output_f) {
                free(node->output);
        }

        free(node);
    }

    free(queue);
}