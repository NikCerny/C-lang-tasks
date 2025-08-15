#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"

#define INIT_CAPACITY_STRINGS 10
#define INIT_CAPACITY_STRING_LEN 10

typedef struct {
    char * string;
} S_STRINGS;

void debugPrint(const S_STRINGS * strings, const int len, FILE *out) {
    printf("Total string count:%d\n", len);
    for(int i = 0; i < len; i++) {
        fprintf(out,"%d: %s\n", i, strings[i].string);
    }
}

void freeArr(S_STRINGS * strings, const int len) {
    for (int i = 0; i < len; i++)
        free(strings[i].string);
    free(strings);
}

S_STRINGS * arrAllocInitial () {
    S_STRINGS *strings = (S_STRINGS *) malloc(INIT_CAPACITY_STRINGS * sizeof(S_STRINGS));
    if (strings == NULL) {
        fprintf(stderr, "Initial memory for \"S_STRINGS *strings\" allocation has failed\n");
        return NULL;
    }
    return strings;
}

char * readStringDynamic (FILE * ptr) {
    int string_len_capacity = INIT_CAPACITY_STRING_LEN;
    int string_len = 0;
    char *string = (char *) malloc(string_len_capacity);
    if (!string) return NULL;
    char ch;

    //skip white chars or not alphanumeric chars
    while ((ch = fgetc(ptr)) != EOF && (isspace(ch) || !isalnum(ch))) {}
    if (ch == EOF) {
        free(string);
        return NULL;
    }

    // read chars until sentence terminator
    do {
        if (string_len + 2 >= string_len_capacity) { // // +2 for punctuation + '\0'
            string_len_capacity *= 2;
            char *temp = (char *) realloc(string, string_len_capacity);
            if (!temp) {
                free(string);
                return NULL;
            }
            string = temp;
        }
        if (ch == '.' || ch == '?' || ch == '!') {
            string[string_len++] = ch; // keep the punctuation
            break;
        }
        string[string_len++] = ch;
    } while ((ch = fgetc(ptr)) != EOF);


    string[string_len] = '\0';
    return string;
}

int readTextDynamic(S_STRINGS ** strings, FILE * input) {

    char * w;
    int text_capacity = INIT_CAPACITY_STRINGS;
    int string_count = 0;
    while ((w = readStringDynamic(input)) != NULL) {
        if (w[0] == '\0') {
            free(w);
            continue;
        }
        if (string_count >= text_capacity) {
            text_capacity *= 2;
            S_STRINGS *temp = (S_STRINGS *)realloc(*strings, text_capacity * sizeof(S_STRINGS));
            if (temp == NULL) {
                fprintf(stderr, "Memory reallocation for \"S_STRINGS *temp\" has failed\n");
                freeArr(*strings, string_count);
                fclose(input);
                return -1;
            }
            *strings = temp;
        }

        (*strings)[string_count].string = w;
        string_count++;
    }

    return string_count;
}


// void uniqWordCount (S_WORDS * words, int len_arr) {
//     int i = 0;
//     while ( i < len_arr ) {
//         int j = i + 1;
//         words[i].count = 1;
//         for( ; j < len_arr; j++) {
//             if (! strcmp(words[i].word, words[j].word))
//                 words[i].count++;
//             else
//                 break;
//         }
//         i = j;
//     }
// }


int compareByString(const void * a, const void * b) {
    S_STRINGS const * w1 = (S_STRINGS *)a;
    S_STRINGS const * w2 = (S_STRINGS *)b;
    return strcasecmp(w1->string, w2->string); // case-insensitive sorting. example: Zebra - apple -> apple - Zebra
}

// int compareByCountDesc(const void * a, const void * b) {
//     S_WORDS const * w1 = (S_WORDS *)a;
//     S_WORDS const * w2 = (S_WORDS *)b;
//     return w2->count > w1->count ? 1 : w2->count < w1->count ? -1 : 0;
// }

void printStrings(const S_STRINGS *strings, const int end, FILE *out) {
    for (int i = 0; i < end; i++)
        fprintf(out, "%s\n", strings[i].string);
}

int main (int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s input_name.txt [string_count_output]\n", argv[0]);
        return 1;
    }

    FILE * input = NULL, *output = stdout;
    const char * inputFile = argv[1];
    input = fopen(inputFile, "r");
    if (input == NULL) {
        perror("Input file cannot be opened");
        return 1;
    }

    output = fopen("output.txt", "w");
    if (output == NULL) {
        perror("Output file cannot be opened");
        fclose(input);
        return 1;
    }

    S_STRINGS * strings = arrAllocInitial();
    if (strings == NULL )
        return 1;
    int string_count = readTextDynamic(&strings, input);
    if (string_count == -1) return 1;

    qsort(strings, string_count, sizeof(S_STRINGS), compareByString);
   // uniqWordCount(strings, word_count);
   // qsort(strings, word_count, sizeof(S_STRINGS), compareByCountDesc);

    int string_count_output;
    if (argc > 2)
        string_count_output = atoi(argv[2]); // user input
    else
        string_count_output = string_count; // list every sentence by default
    if (string_count_output > string_count) // check user input
        string_count_output = string_count;
    printStrings(strings, string_count_output,output);
  //  debugPrint(strings, string_count_output,output);
    fclose(input);
    fclose(output);
    freeArr(strings,string_count);
    return 0;
}

