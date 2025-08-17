#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY_INT 10

int * initialAlloc() {
    int * res = (int *) malloc (INIT_CAPACITY_INT * sizeof(res));
    if (res == NULL) {
        fprintf(stderr, "Initial memory for \"int * res\" allocation has failed\n");
        return NULL;
    }
    return res;
}


int main(int argc, char ** argv) {

    if (argc < 1) {
        fprintf(stderr, "Usage: %s input_name.txt\n", argv[0]);
        return 1;
    }
    FILE * input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Input file cannot be opened");
        return 1;
    }

    int * res = initialAlloc();

    free(res);
    fclose(input);
    return 0;
}
