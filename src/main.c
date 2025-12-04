#include <stdio.h>
#include "hashtable.h"
#include "toxic_loading.h"
#include "aho.h"
#include "string_handling.h"
#include "file_handling.h"


// initiliase hashtables
// table = toxic dictionary
// table1 = stopwords
// table2 = temporary unique words dictionary
HashItem* table = NULL;
HashItem* table1 = NULL;
HashItem* table2 = NULL;


// function definitions
void out_general();
void out_top_n();
int get_choice1();
int get_next_record_num();
void create_outfile();
void get_choice2();
void reset_ss();

int main() {

    // load toxic words dictionary with words from severe_words.txt and mild_words.txt
    // load stopwords dictionary with words from stopword.txt
    printf("Loading severe_words.txt...\n");
    load_toxic(&table, "data/severe_words.txt");
    printf("Loading mild_words.txt...\n");
    load_toxic(&table, "data/mild_words.txt");
    printf("loading stop_word.txt\n");
    load_toxic(&table1, "data/stopword.txt");

    // build aho trie
    printf("Building Aho trie...\n\n");
    ACNode* automation = ac_build(table);

    // infinite loop
    while (1) {
        // if get_choice returns 0 then break
        if (!get_choice1()) break;

        // getting file for input analysis, analyse file
        process_file(automation, table);

        // get num unique words
        ss.num_unique = get_num_elements(table2);
        // get num unique toxic words, mild words, severe words
        num_unique_gtz_grp(table);

        // choose between analyse new file, general stats, output top n, output to file
        get_choice2();

        // reset all items in dictionary table to 0
        reset_table(table);

        // free all items in dictionary table2
        free_table(&table2);

        // reset values in struct ss
        reset_ss();
    }


    // free and exit
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


void reset_ss() {
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


// prints stringstats / general stats
void out_general() {
    printf("\nGeneral Statistics:\n");
    print_stringstats();
    printf("\n");
}


// gets input n
// sorts dictionary, outputs top n
void out_top_n() {
    int n;
    while (1) {
        printf("Input n: ");
        scanf("%d", &n);
        getchar();
        int len = get_num_elements(table);
        if (n > 0 && n <= len) {
            break;
        }
        printf("invalid input for n, n > 0 and n <= length of hashmap\n");
    }
    Item* item_arr = get_top_n(&table, n);
    printf("Top n Toxic Words:\n");
    print_top_n(item_arr, n);
    printf("\n");
}

// gets input from user, if 0 then exit, if 1 then analyse a file
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

// get number of record for outputting to file
int get_next_record_num() {
    FILE* f = fopen("analysis_report.txt", "r");
    if (!f) return 1;

    int count = 0;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Record", 6) == 0) {
            count++;
        }
    }
    fclose(f);
    return count + 1;
}


// appends to analysis_report.txt the current analysis of the file,
// outputs the file name, general stats (string stats) and top 10 toxic words found
void create_outfile() {
    int record_num = get_next_record_num();

    FILE* fptr = fopen("analysis_report.txt", "a");
    if (!fptr) {
        printf("Error opening output file for appending\n");
        return;
    }

    fprintf(fptr, "Record %d: %s\n", record_num, ss.filename);
    fprintf(fptr, "------------------------------------------------\n");

    float avg_word_precord = (float)ss.word_count / ss.num_records;
    float avg_word_len = (float)ss.total_length / ss.word_count;
    float ratio_w = (float)ss.num_unique / ss.word_count;
    float ratio_ts = (float)ss.severe_unique / ss.toxic_unique;
    float ratio_tm = (float)ss.mild_unique / ss.toxic_unique;

    fprintf(fptr, "StringStats:\n");
    fprintf(fptr, "Total Number of Words(excluding stopwords): %d\n", ss.word_count);
    fprintf(fptr, "Total Number of Characters(excluding stopwords): %d\n", ss.total_length);
    fprintf(fptr, "Total Number of unique words(excluding stopwords): %d\n", ss.num_unique);
    fprintf(fptr, "Total Number of Sentences: %d\n", ss.num_sentences);
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
        fprintf_top_n(item_arr, n, fptr);
    }
    else {
        fprintf(fptr, "No toxic words found.\n");
    }

    fprintf(fptr, "\n================================================\n\n");

    printf("Successfully appended Record %d to analysis_report.txt\n\n", record_num);
    fclose(fptr);
}

// get input from user
// if input == 0 then analyse a new file
// if input == 1 then output general stats
// if input == 2 then output top n]
// if input == 3 then append to analysis_report.txt 
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