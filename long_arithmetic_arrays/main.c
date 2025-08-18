#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int * number;
    int len; // number of digits stored
    int sign; // +1 - positive, -1 - negative
    int point_pos; // number of digits after decimal point
} S_NUMBER;

void freeStruct(S_NUMBER * n) {
    if (n == NULL) return;
    free(n->number);
    free(n);
}

void trimZeros(S_NUMBER *n) {
    // 1. Remove leading zeros (before the decimal point)
    int start = 0;
    while (start < n->len - 1 - n->point_pos && n->number[start] == 0)
        start++;

    // Shift digits to the left
    if (start > 0) {
        for (int i = start; i < n->len; ++i)
            n->number[i - start] = n->number[i];
        n->len -= start;
    }

    // 2. Remove trailing zeros in the decimal part
    while (n->point_pos > 0 && n->len > 1 && n->number[n->len - 1] == 0) {
        n->len--;
        n->point_pos--;
    }

    // If the number remains completely zero, reset to 0
    int is_zero = 1;
    for (int i = 0; i < n->len; i++) {
        if (n->number[i] != 0) {
            is_zero = 0;
            break;
        }
    }

    if (is_zero) {
        n->len = 1;
        n->point_pos = 0;
        n->sign = +1;
        n->number[0] = 0;
    }
}

void printDebug(const S_NUMBER * n) {
    if (n->sign == -1)
        printf("-");
    for (int i = 0; i < n->len; i++) {
        if (i == n->len - n->point_pos)
            printf(".");
        printf("%d", n->number[i]);
    }
    puts("");
    printf("point pos = %d\n", n->point_pos);
    printf("len = %d\n", n->len);
    printf("sign = %d\n", n->sign);
}

void printNumberToFile(const S_NUMBER * n, FILE *out) {
    if (n == NULL) return;
    if (n->sign == -1)
        fprintf(out, "-");
    for (int i = 0; i < n->len; i++) {
        if (i == n->len - n->point_pos)
            fprintf(out, ".");
        fprintf(out, "%d", n->number[i]);
    }
    fprintf(out, "\n");
}

int getMax(int a, int b) {
    return a > b ? a : b;
}

void addArr(const S_NUMBER *a, const S_NUMBER *b, S_NUMBER *res) {
    int carry = 0;
    int i = a->len - 1;
    int j = b->len - 1;
    int k = res->len - 1;
    for(; i>=0 && j>=0;i--,j--,k--) {
        int x = a->number[i] + b->number[j] + carry;
        res->number[k] = (x % 10);
        carry = x / 10;
    }
    for(;i>=0;i--,k--) {
        int x = a->number[i] + carry;
        res->number[k] = (x % 10);
        carry = x / 10;
    }
    for(;j>=0;j--,k--) {
        int x = b->number[j] + carry;
        res->number[k] = (x % 10);
        carry = x / 10;
    }
    res->number[k] = carry;
}

void substractArr(const S_NUMBER *a, const S_NUMBER *b, S_NUMBER *res) {
    int borrow = 0;
    int i = a->len - 1;
    int j = b->len - 1;
    int k = res->len - 1;
    for(; i>=0 && j>=0;i--,j--,k--) {
        int x = a->number[i] - b->number[j] + borrow;
        res->number[k] = (x + 10) % 10;
        borrow = x < 0 ? -1 : 0;
    }
    for(;i>=0;i--,k--) {
        int x = a->number[i] + borrow;
        res->number[k] = (x + 10) % 10;
        borrow = x < 0 ? -1 : 0;
    }
    while (k >= 0)
        res->number[k--] = 0;
}

S_NUMBER * arrAlloc(const int len) {
    S_NUMBER * newArr = (S_NUMBER *) malloc(1 * sizeof(S_NUMBER));
    if (newArr == NULL) {
        fprintf(stderr, "Memory allocation has failed\n");
        return NULL;
    }
    newArr->number = (int *) calloc(len,sizeof(int));
    if (newArr->number == NULL) {
        fprintf(stderr, "Memory allocation has failed\n");
        free(newArr);
        return NULL;
    }
    newArr->len = len;
    newArr->sign = +1;
    newArr->point_pos = 0;
    return newArr;
}


int cmpNumbersBigEndianAbsValue(const S_NUMBER * a, const S_NUMBER * b) {
    if (a->len > b->len)
        return 1;
    if (a->len < b->len)
        return -1;
    for (int i = 0; i<a->len; i++) {
        if (a->number[i] > b ->number[i])
            return 1;
        if (a->number[i] < b ->number[i])
            return -1;
    }
    return 0;
}

S_NUMBER * copyAndNormalizeRealNum(const S_NUMBER * a, int len) {
    S_NUMBER * tmp = arrAlloc(len);
    if (tmp == NULL)
        return NULL;
    tmp->len = len;
    tmp->point_pos = a->point_pos;
    tmp->sign = a->sign;
    int i = 0;
    for(; i < a->len; i++)
        tmp->number[i] = a->number[i];
    for(; i < tmp->len;i++)
        tmp->number[i] = 0;
    return tmp;
}

S_NUMBER * addArrStart(const S_NUMBER * a,const S_NUMBER * b) {
    S_NUMBER * tmpA = NULL;
    S_NUMBER * tmpB = NULL;

    // Align decimal places
    if (a->point_pos < b->point_pos) {
        tmpA = copyAndNormalizeRealNum(a, a->len + b->point_pos - a->point_pos);
        if (tmpA == NULL) return NULL;
        a = tmpA;
        tmpA->point_pos = b->point_pos;
    } else if (a->point_pos > b->point_pos) {
        tmpB = copyAndNormalizeRealNum(b, b->len + a->point_pos - b->point_pos);
        if (tmpB == NULL) {
            freeStruct(tmpA);
            return NULL;
        }
        b = tmpB;
        tmpB->point_pos = a->point_pos;
    }

    int len_res = getMax(a->len, b->len) + 1;
    S_NUMBER * res = arrAlloc(len_res);
    if (res == NULL) {
        freeStruct(tmpA);
        freeStruct(tmpB);
        return NULL;
    }
    res->point_pos = a->point_pos;

    int c = cmpNumbersBigEndianAbsValue(a, b);
    if (a->sign == b->sign) {
        res->sign = a->sign;
        addArr(a, b, res);
    } else {
        if (c == 0) {
            res->sign = +1;
            // result is zero
        } else if (c > 0) {
            res->sign = a->sign;
            substractArr(a, b, res);
        } else {
            res->sign = b->sign;
            substractArr(b, a, res);
        }
    }

    freeStruct(tmpA);
    freeStruct(tmpB);

    return res;
}

void multiplyArr(const S_NUMBER * a, const S_NUMBER * b, const S_NUMBER * res) {

    for (int i = 0; i < res->len; i++)
        res->number[i] = 0;

    for (int i = a->len - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = b->len - 1; j >= 0; j--) {
            int pos = i + j + 1;
            int temp = res->number[pos] + a->number[i] * b->number[j] + carry;
            res->number[pos] = temp % 10;
            carry = temp / 10;
        }
        res->number[i] += carry;
    }
}


S_NUMBER * multiplyArrStart( const S_NUMBER *a,  const S_NUMBER *b) {
    const int len_res = a->len+b->len;
    S_NUMBER * res = arrAlloc(len_res);
    if (res == NULL)
        return NULL;
    multiplyArr(a,b,res);
    res->sign = (a->sign == b->sign) ? +1 : -1;
    res->point_pos = a->point_pos + b->point_pos;
    return res;
}

S_NUMBER * getNumber(FILE * ptr) {
    int capacity = 10;
    int ch;
    S_NUMBER * n = (S_NUMBER *) malloc(1 * sizeof(S_NUMBER));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation has failed\n");
        return NULL;
    }
    n->number = (int *) calloc(capacity,sizeof(int));
    if (n->number == NULL) {
        fprintf(stderr, "Memory allocation has failed\n");
        free(n);
        return NULL;
    }
    n->len = 0;
    n->point_pos = 0;

    while ((ch = fgetc(ptr)) != EOF && isspace(ch)) {}
    switch(ch) {
        case EOF:
            freeStruct(n);
            return NULL;
        case '-':
            n->sign = -1;
            ch = fgetc(ptr);
            break;
        case '+':
            n->sign = +1;
            ch = fgetc(ptr);
            break;
        default:
            n->sign = +1;
    }

    do {
        if (!isdigit(ch)) {
            if (ch == '.' && n->point_pos == 0) {
                n->point_pos = n->len;
                continue;
            }
            if (ch != EOF)
                fprintf(stderr, "Error: this character is not a valid digit in file: %c\n", ch);
            freeStruct(n);
            return NULL;
        }
        if (n->len + 1 >= capacity) {
            capacity *= 2;
            int * tmp = (int *) realloc(n->number, capacity*sizeof(int));
            if (tmp == NULL) {
                fprintf(stderr, "Memory allocation has failed\n");
                freeStruct(n);
                return NULL;
            }
            n->number = tmp;
        }
        n->number[n->len++] = ch - '0';
    } while ((ch = fgetc(ptr)) != EOF && !isspace(ch));


    if (n->point_pos != 0)
        n->point_pos = n->len - n->point_pos;

    return n;
}

int main(int argc, char ** argv) {

    if (argc < 3) {
        fprintf(stderr, "Usage: %s input_name.txt {sum,prod}\n", argv[0]);
        return 1;
    }

    FILE * input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Input file cannot be opened");
        return 1;
    }
    FILE * output = fopen("output.txt", "w");
    if (output == NULL) {
        fclose(input);
        perror("Output file cannot be opened");
        return 1;
    }

    S_NUMBER * (*operation) (const S_NUMBER *, const S_NUMBER *) = NULL;

    if (! strcmp(argv[2], "sum")) {
        operation = addArrStart;
    }
    else if (! strcmp(argv[2], "prod")) {
        operation = multiplyArrStart;
    }
    else {
        fprintf(stderr, "Invalid operation: %s. Use 'sum' or 'prod'\n", argv[2]);
        fclose(input);
        fclose(output);
        return 1;
    }

    S_NUMBER * res = NULL, * next_number = NULL;
    res = getNumber(input);
    if (res == NULL) {
        fclose(input);
        fclose(output);
        fprintf(stderr, "Error: no number on input file\n");
        return 1;
    }

    while (1) {

        next_number = getNumber(input);
        if (next_number == NULL)
            break;

        S_NUMBER * new_res = operation(res, next_number);

        if (new_res == NULL) {
            freeStruct(next_number);
            freeStruct(res);
            fclose(input);
            fclose(output);
            return 1;
        }
        freeStruct(res);
        res = new_res;
        trimZeros(res);
        freeStruct(next_number);
        next_number = NULL;
    }

    printNumberToFile(res, output);
    freeStruct(next_number);
    freeStruct(res);
    fclose(input);
    fclose(output);
    return 0;
}
