#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

#define INIT_CAPACITY_WORDS 1000
#define INIT_CAPACITY_CHARS 1000

typedef struct {
    char * word;
    int count;
} S_WORDS;

void debugPrint(S_WORDS * words, int len, FILE *out) {
    printf("Total word count:%d\n", len);
    for(int i = 0; i < len; i++) {
        fprintf(out,"%s\n", words[i].word);
    }
}

void freeArr(S_WORDS * words, const int len) {
    for (int i = 0; i < len; i++)
        free(words[i].word);
    free(words);
}

S_WORDS * arrAllocInitial () {
    S_WORDS *words = (S_WORDS *) malloc(INIT_CAPACITY_WORDS * sizeof(S_WORDS));
    if (words == NULL) {
        fprintf(stderr, "Initial memory for \"S_WORDS *words\" allocation has failed\n");
        return NULL;
    }
    return words;
}

int copyWord(S_WORDS * words, FILE * f_ptr) {
    int words_capacity = INIT_CAPACITY_WORDS;
    int word_len_capacity = INIT_CAPACITY_CHARS;
    int word_idx = 0, char_idx = 0;
    char ch = fgetc(f_ptr);


    words[word_idx].word = (char *) malloc(word_len_capacity);
    while (ch != EOF ) {
        if (word_idx >= words_capacity) {
            words_capacity *= 2;
            S_WORDS *temp = (S_WORDS *)realloc(words, words_capacity * sizeof(words));
            if (temp == NULL) {
                fprintf(stderr, "Memory reallocation for \"S_WORDS *temp\" has failed\n");
                freeArr(words, word_idx);
                return 1;
            }
            words = temp;
        }

        int word_len = 0;
        while( ch && isalnum(ch)) {
            if (word_len + 1 >= word_len_capacity) {
                word_len_capacity *= 2;
                char *temp = (char *) realloc(words[word_idx].word, word_len_capacity);
                if (!temp) {
                    free(words[word_idx].word);
                    return NULL;
                }
                words[word_idx].word = temp;
            }
            words[word_idx].word[char_idx] = tolower(ch);
            word_len++;
            char_idx++;
            ch = fgetc(f_ptr);
        }
        words[word_idx].word[char_idx] = '\0';
        if (char_idx > 0) {
            word_idx++;
            words[word_idx].word = (char *) malloc(word_len_capacity);
        }
        char_idx = 0;
        if (ch == '\0')
            break;
        ch = fgetc(f_ptr);
    }

    return word_idx;
}

int wordCount(S_WORDS * words, int len) {
    int count_max = 0;
    for(int i = 0; i < len; i++) {
        words[i].count = 1;
        for (int j = i + 1; j < len; j++)
            if (! strcmp(words[i].word, words[j].word))
                words[i].count++;
        if ( count_max < words[i].count)
            count_max = words[i].count;
    }
    return count_max;
}


int main () {

    FILE * input = NULL, *output = stdout;
    const char * inputFile = "input.txt";
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


    S_WORDS * words = arrAllocInitial();
    if (words == NULL )
        return 1;

    int len = copyWord(words,input);
     debugPrint(words,len,output);

    // int count_max = wordCount(words,len);
    // for(int i = 0; i < len; i++) {
    //     if (count_max == words[i].count)
    //         printf("%s = %d\n", words[i].word, words[i].count);
    // }

    freeArr(words,len+1);
    return 0;
}
