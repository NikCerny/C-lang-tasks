#include <ctype.h>
#include "stdio.h"

#define MAX_WORDS 1000
#define MAX_CHARS 1000

typedef struct {
    char word[MAX_CHARS];
    int count = 0;
} S_WORDS;


int copyWord(S_WORDS * words, const char * src) {
    int word_idx = 0, char_idx = 0;
    while (*src) {
        while( *src && isalnum(*src)) {
            words[word_idx].word[char_idx] = *src;
            char_idx++;
            src++;
        }
        words[word_idx].word[char_idx] = '\0';
        if (char_idx > 0)
            word_idx++;
        char_idx = 0;
        src++;
    }
    return word_idx;
}

void debugPrint(S_WORDS * words, int len) {
    printf("Total word count:%d\n", len);
    for(int i = 0; i < len; i++) {
        printf("%s\n", words[i].word);
    }
}


int main () {

    const char * text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
    const char * ptr_text = text;
    S_WORDS words[MAX_WORDS];

    int len = copyWord(words,ptr_text);
    debugPrint(words,len);

    return 0;
}
