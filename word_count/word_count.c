#include <ctype.h>
#include "stdio.h"

#define MAX_WORDS 1000
#define MAX_CHARS 1000

typedef struct {
    char word[MAX_CHARS];
    int count = 0;
} S_WORDS;





int main () {

    const char * text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum";
    const char * ptr_text = text;
    S_WORDS words[MAX_WORDS];

    int i = 0, j = 0;
    while (*ptr_text) {
        while( *ptr_text && ! isspace(*ptr_text)) {
         words[i].word[j] = *ptr_text;
            j++;
            ptr_text++;
        }
        words[i].word[j] = '\0';
        j = 0;
        i++;
        ptr_text++;
    }


    for(int n = 0; n < i; n++) {
        printf("%s\n", words[n].word);
    }

    return 0;
}
