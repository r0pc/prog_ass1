#include <stdio.h>
#include "hashtable.h"
#include "toxic_loading.h"
#include "aho.h"
#include "string_handling.h"
#include "file_handling.h"

const char* filename_main = "data/chatlogs.csv";
HashItem* table = NULL;
HashItem* table1 = NULL;
HashItem* table2 = NULL;

void out_general();
void out_top_n();
int get_choice1();
int get_next_record_num();
void create_outfile();
void get_choice2();
void reset_ss();

int main() {

    printf("Loading severe_words.txt...\n");
    load_toxic(&table, "data/severe_words.txt");
    printf("Loading mild_words.txt...\n");
    load_toxic(&table, "data/mild_words.txt");
    printf("loading stop_word.txt\n");
    load_toxic(&table1, "data/stopword.txt");

    printf("Building Aho trie...\n\n");
    ACNode* automation = ac_build(table);

    while (1) {
        if (!get_choice1()) break;
        process_file(automation, table);
        ss.num_unique = get_num_elements(table2);
        num_unique_gtz_grp(table);
        get_choice2();

        reset_table(table);
        free_table(&table2);
        reset_ss();
    }

    // printf("clearing hash tale...\n");
    // reset_table(table);

    printf("freeing Aho trie...\n");
    ac_free(automation);
    printf("freeing toxic words table...\n");
    free_table(&table);
    printf("freeing stopwords table...\n");
    free_table(&table1);
    printf("freeing unique words table...\n");
    free_table(&table2);
    printf("exiting programme...\n");

    return 0;
}

void reset_ss(){
    strcpy(ss.filename, "");
    ss.mild_total = 0;
    ss.mild_unique = 0;
    ss.num_records = 0;
    ss.num_unique = 0;
    ss.severe_total = 0;
    ss.severe_unique = 0;
    ss.total_length = 0;
    ss.toxic_unique = 0;
    ss.word_count = 0;    
}



void out_general() {
    printf("\nGeneral Statistics:\n");
    print_stringstats();
    printf("\n");
}

void out_top_n() {
    int n;
    printf("Input n: ");
    scanf("%d", &n);
    getchar();
    Item* item_arr = get_top_n(&table, n);
    printf("Top n Toxic Words:\n");
    print_top_n(item_arr, n);
    printf("\n");
}

int get_choice1() {
    int c1;
    printf("0. Exit the program\n");
    printf("1. Analyse a file\n");
    while (1) {
        printf("Enter choice: ");
        scanf("%d", &c1);
        getchar();
        if (c1 == 0 || c1 == 1) {
            printf("\n");
            break;
        }
        printf("Invalid input choice, Please try again\n");
    }
    return c1;
}

int get_next_record_num() {
    FILE* f = fopen("output.txt", "r");
    if (!f) return 1; // File doesn't exist yet, start at 1

    int count = 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // Check if the line starts with "Record"
        if (strncmp(line, "Record", 6) == 0) {
            count++;
        }
    }
    fclose(f);
    return count + 1;
}

void create_outfile(){
    int record_num = get_next_record_num();

    FILE* fptr = fopen("output.txt", "a");
    if (!fptr) {
        printf("Error opening output file for appending\n");
        return;
    }

    fprintf(fptr, "Record %d: %s\n", record_num, ss.filename);
    fprintf(fptr, "------------------------------------------------\n");

    float avg_word_precord = (float) ss.word_count / ss.num_records;
    float avg_word_len = (float) ss.total_length / ss.word_count;
    float ratio_w = (float)ss.num_unique / ss.word_count;
    float ratio_ts = (float)ss.severe_unique/ss.toxic_unique;
    float ratio_tm = (float)ss.mild_unique/ss.toxic_unique;

    fprintf(fptr, "StringStats:\n");
    fprintf(fptr, "Total Number of Words(excluding stopwords): %d\n", ss.word_count);
    fprintf(fptr, "Total Number of Characters(excluding stopwords): %d\n", ss.total_length);
    fprintf(fptr, "Total Number of unique words(excluding stopwords): %d\n", ss.num_unique);
    fprintf(fptr, "Ratio of total words with unique words: %.3f\n", ratio_w);
    fprintf(fptr, "Average word length: %.2f\n", avg_word_len);
    fprintf(fptr, "Average Number of words per record: %.2f\n", avg_word_precord);
    fprintf(fptr, "Total Number of toxic words: %d\n", (ss.mild_total + ss.severe_total));
    fprintf(fptr, "Total Number of mild Words: %d\n", ss.mild_total);
    fprintf(fptr, "Total Number of severe words: %d\n", ss.severe_total);
    fprintf(fptr, "Total Number of unique severe words: %d\n", ss.severe_unique);
    fprintf(fptr, "Total Number of unique mild words: %d\n", ss.mild_unique);
    fprintf(fptr, "Ratio of mild unique to total toxic unique: %.2f\n", ratio_tm);
    fprintf(fptr, "Ratio of severe unique to total toxic unique: %.2f\n", ratio_ts);
    fprintf(fptr, "\n");

    fprintf(fptr, "Top 10 Toxic Words:\n");

    int n = 10;
    Item* item_arr = get_top_n(&table, n);

    if (item_arr != NULL) {
        for (int i = 0; i < n; i++) {
            if (item_arr[i].val == 0 && strlen(item_arr[i].key) == 0) break;
            fprintf(fptr, "%d. %s -> %d\n", i + 1, item_arr[i].key, item_arr[i].val);
        }
        free(item_arr);
    }
    else {
        fprintf(fptr, "No toxic words found.\n");
    }

    fprintf(fptr, "\n================================================\n\n");

    printf("Successfully appended Record %d to output.txt\n\n", record_num);
    fclose(fptr);
}

void get_choice2() {
    int c2 = -1;
    printf("\n0. Analyse a new file\n");
    printf("1. Display general word statistics\n");
    printf("2. Display top n toxic words\n");
    printf("3. Create an output file\n");
    while (1) {
        printf("Enter Choice: ");
        scanf("%d", &c2);
        getchar();
        if (c2 == 0) {
            printf("\n");
            break;
        }
        switch (c2)
        {
        case 1:
            out_general();
            break;
        case 2:
            out_top_n();
            break;
        case 3:
            create_outfile();
            break;
        default:
            printf("Invalid input choice, Please try again\n");
            break;
        }
    }
}