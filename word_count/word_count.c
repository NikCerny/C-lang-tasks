#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "stdio.h"

#define INIT_CAPACITY_WORDS 1000
#define MAX_CHARS 1000

typedef struct {
    char word[MAX_CHARS];
    int count;
} S_WORDS;

void debugPrint(S_WORDS * words, int len) {
    printf("Total word count:%d\n", len);
    for(int i = 0; i < len; i++) {
        printf("%s\n", words[i].word);
    }
}

int copyWord(S_WORDS * words, FILE * f_ptr) {
    int word_idx = 0, char_idx = 0;
    char ch = fgetc(f_ptr);
    while (ch != EOF ) {
        while( ch && isalnum(ch)) {
            words[word_idx].word[char_idx] = tolower(ch);
            char_idx++;
            ch = fgetc(f_ptr);
        }
        words[word_idx].word[char_idx] = '\0';
        if (char_idx > 0)
            word_idx++;
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

S_WORDS * arrAllocInitial () {
    S_WORDS *words = (S_WORDS *) malloc(INIT_CAPACITY_WORDS * sizeof(S_WORDS));
    if (words == NULL) {
        fprintf(stderr, "Initial memory allocation has failed\n");
        return NULL;
    }
    return words;
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
   // debugPrint(words,len);

    int count_max = wordCount(words,len);
    for(int i = 0; i < len; i++) {
        if (count_max == words[i].count)
            printf("%s = %d\n", words[i].word, words[i].count);
    }

    free(words);
    return 0;
}
