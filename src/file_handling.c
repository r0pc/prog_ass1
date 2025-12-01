#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashtable.h"
#include "aho.h"
#include "string_handling.h"
#include "file_handling.h"

FILE* get_file(int* filetype) {
    char buff[64];
    FILE* fptr;
    while (1) {
        while (1) {
            printf("input file name to analyse: ");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff, "\n\r")] = '\0';
            if (check_filetype(buff)) {
                break;
            }
            printf("invalid file name, only .txt or .csv allowed\n");
        }
        fptr = fopen(buff, "r");
        if (!fptr) {
            printf("file not found, reenter file name\n");
        }
        else {
            *filetype = check_filetype(buff);
            strcpy(ss.filename, buff);
            break;
        }
    }
    return fptr;
}

int check_filetype(char* filename) {
    int len = strlen(filename);
    if (len >= 4) {
        const char* last_four_ptr = &filename[len - 4];
        if (strcmp(last_four_ptr, ".txt") == 0) {
            return 1;
        }
        else if (strcmp(last_four_ptr, ".csv") == 0) {
            return 2;
        }
    }

    return 0;
}

void read_txt(FILE* fptr, ACNode* automation, HashItem* table) {
    char buffer[2056];
    while (fgets(buffer, sizeof(buffer), fptr)) {
        process_string(automation, table, buffer);
    }
}


char* csv_get_col(char* row, int target_col) {
    int col = 0;
    int inside_quotes = 0;
    char* start = row;

    for (char* p = row; ; p++) {
        char c = *p;

        if (c == '"') {
            inside_quotes = !inside_quotes;
        }
        else if ((c == ',' || c == '\0' || c == '\n') && !inside_quotes) {
            // end of field
            if (col == target_col) {
                if (c != '\0' && c != '\n')
                    *p = '\0';   // terminate the field
                return start;
            }
            col++;
            start = p + 1;

            if (c == '\0' || c == '\n')
                break;
        }
    }
    return NULL;
}


void read_csv(FILE* fptr, ACNode* automation, HashItem* table) {
    char buffer[4096];

    // Read header line
    if (!fgets(buffer, sizeof(buffer), fptr)) {
        return;
    }

    // Count columns based on commas
    int col_count = 1;
    for (char* p = buffer; *p; p++) {
        if (*p == ',') {
            col_count++;
        }
    }

    // Ask user for column number (0-based)
    int target_col = -1;
    while (target_col < 0 || target_col >= col_count) {
        printf("Enter column number to analyze (0 to %d): ", col_count - 1);
        scanf("%d", &target_col);

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    // Process each row normally
    while (fgets(buffer, sizeof(buffer), fptr)) {
        char row_copy[4096];
        strcpy(row_copy, buffer);

        char* field = csv_get_col(row_copy, target_col);
        if (!field) continue;

        size_t len = strlen(field);
        if (len >= 2 && field[0] == '"' && field[len - 1] == '"') {
            field[len - 1] = '\0';
            field++;
        }

        process_string(automation, table, field);
    }
}



void process_file(ACNode* automation, HashItem* table) {
    int filetype = 0;
    FILE* fptr = get_file(&filetype);
    printf("Processing File...\n");
    if (filetype == 1) {
        read_txt(fptr, automation, table);
    }
    else if (filetype == 2) {
        read_csv(fptr, automation, table);
    }
    else {
        perror("invalid file type, terminating program");
        exit(EXIT_FAILURE);
    }
}