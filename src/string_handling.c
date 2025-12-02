#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datastructs.h"
#include "string_handling.h"
#include "aho.h"
#include "hashtable.h"


// normalizes the string s, (lower case + removing excessive spaces)
// also removes stopwords
void normalize_inplace(char* s) {
    char* src = s;
    char* dst = s;

    char current_word[256];
    int word_len = 0;

    int words_processed = 0;
    int first_word_written = 0;

    while (1) {
        unsigned char c = (unsigned char)*src;

        if (isalnum(c)) {
            if (word_len < sizeof(current_word) - 1) {
                current_word[word_len++] = tolower(c);
            }
        }
        else {
            if (word_len > 0) {
                current_word[word_len] = '\0'; // Null terminate the temp word
                if (!find_item(table1, current_word)) {


                    if (first_word_written) {
                        *dst++ = ' ';
                    }

                    char* p = current_word;
                    while (*p) {
                        *dst++ = *p++;
                    }

                    first_word_written = 1;
                    words_processed++;

                    add_item(&table2, current_word, 0, UNIQUE);
                }

                word_len = 0;
            }
            if (c == '\0') break;
        }
        src++;
    }

    *dst = '\0';

    ss.total_length += (dst - s);
    ss.word_count += words_processed;
}

// first normalizes the string then checks if substr exists in the trie
void process_string(ACNode* automation, HashItem* table, char* s) {
    ss.num_records++;
    // printf("Orignal: %s\n", s);
    normalize_inplace(s);
    // printf("Changed: %s\n", s);
    ac_search(automation, s, table);
}

// prints general file info
void print_stringstats() {
    float avg_word_precord = (float)ss.word_count / ss.num_records;
    float avg_word_len = (float)ss.total_length / ss.word_count;
    float ratio_w = (float)ss.num_unique / ss.word_count;
    float ratio_ts = (float)ss.severe_unique / ss.toxic_unique;
    float ratio_tm = (float)ss.mild_unique / ss.toxic_unique;

    printf("Total Number of Words(excluding stopwords): %d\n", ss.word_count);
    printf("Total Number of Characters(excluding stopwords): %d\n", ss.total_length);
    printf("Total Number of unique words(excluding stopwords): %d\n", ss.num_unique);
    printf("Ratio of total words with unique words: %.3f\n", ratio_w);
    printf("Average word length: %.2f\n", avg_word_len);
    printf("Average Number of words per record: %.2f\n", avg_word_precord);
    printf("Total Number of toxic words: %d\n", (ss.mild_total + ss.severe_total));
    printf("Total Number of mild Words: %d\n", ss.mild_total);
    printf("Total Number of severe words: %d\n", ss.severe_total);
    printf("Total Number of unique severe words: %d\n", ss.severe_unique);
    printf("Total Number of unique mild words: %d\n", ss.mild_unique);
    printf("Ratio of mild unique to total toxic unique: %.2f\n", ratio_tm);
    printf("Ratio of severe unique to total toxic unique: %.2f\n", ratio_ts);
}
