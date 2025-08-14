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

void debugPrint(const S_WORDS * words, const int len, FILE *out) {
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

char * readWordDynamic (FILE * ptr) {
    int word_len_capacity = INIT_CAPACITY_CHARS;
    int word_len = 0;
    char *word = (char *) malloc(word_len_capacity);
    if (!word) return NULL;
    int ch;

    //skip white chars
    while ((ch = fgetc(ptr)) != EOF && isspace(ch)) {}
    if (ch == EOF) {
        free(word);
        return NULL;
    }

    // read chars while not EOF or space
    do {
        if (word_len + 1 >= word_len_capacity) {
            word_len_capacity *= 2;
            char *temp = (char *) realloc(word, word_len_capacity);
            if (!temp) {
                free(word);
                return NULL;
            }
            word = temp;
        }
        word[word_len++] = tolower(ch);
    } while ((ch = fgetc(ptr)) != EOF && !isspace(ch));

    word[word_len] = '\0';
    return word;
}

int readWordsDynamic(S_WORDS * words, FILE * input) {

    char * w;
    int words_capacity = INIT_CAPACITY_WORDS;
    int word_count = 0;
    while ((w = readWordDynamic(input)) != NULL) {
        if (w[0] == '\0') {
            free(w);
            continue;
        }
        if (word_count >= words_capacity) {
            words_capacity *= 2;
            S_WORDS *temp = (S_WORDS *)realloc(words, words_capacity * sizeof(S_WORDS));
            if (temp == NULL) {
                fprintf(stderr, "Memory reallocation for \"S_WORDS *temp\" has failed\n");
                freeArr(words, word_count);
                fclose(input);
                return -1;
            }
            words = temp;
        }

        words[word_count].word = w;
        words[word_count].count = 0;
        word_count++;
    }

    return word_count;
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
    int word_count = readWordsDynamic(words, input);
    if (word_count == -1) return 1;
    
    int count_max = wordCount(words,word_count);
    for(int i = 0; i < word_count; i++) {
        if (count_max == words[i].count)
            fprintf(output,"%s = %d\n", words[i].word, words[i].count);
    }

    freeArr(words,word_count);
    return 0;
}
