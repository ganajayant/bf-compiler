#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 30000

unsigned char Memory[SIZE] = {0};
int CurrentIndex = 0;

void moveright() {
    if (CurrentIndex + 1 >= SIZE) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    CurrentIndex++;
}

void moveleft() {
    if (CurrentIndex == 0) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    CurrentIndex--;
}

void increment(int index) {
    if (index >= SIZE || index < 0) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    Memory[index]++;
}

void decrement(int index) {
    if (index >= SIZE || index < 0) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    Memory[index]--;
}

void output(int index) {
    if (index >= SIZE || index < 0) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    putchar((char)Memory[index]);
}

void input(int index) {
    if (index >= SIZE || index < 0) {
        fprintf(stderr, "out of bound");
        exit(-1);
    }
    char ch = getchar();
    Memory[index] = ch;
}

int find_matching_bracket(char *code, int start, int direction) {
    int depth = 0;
    int length = strlen(code);
    for (int i = start; i >= 0 && i < length; i += direction) {
        if (code[i] == '[')
            depth += direction;
        if (code[i] == ']')
            depth -= direction;
        if (depth == 0)
            return i;
    }
    fprintf(stderr, "unmatched brackets\n");
    exit(-1);
}

void Operation(char *code, int *pc) {
    char ch = code[*pc];
    switch (ch) {
    case '>':
        moveright();
        break;
    case '<':
        moveleft();
        break;
    case '+':
        increment(CurrentIndex);
        break;
    case '-':
        decrement(CurrentIndex);
        break;
    case '.':
        output(CurrentIndex);
        break;
    case ',':
        input(CurrentIndex);
        break;
    case '[':
        if (Memory[CurrentIndex] == 0) {
            *pc = find_matching_bracket(code, *pc, 1);
        }
        break;
    case ']':
        if (Memory[CurrentIndex] != 0) {
            *pc = find_matching_bracket(code, *pc, -1);
        }
        break;
    default:
        break;
    }
}

void read(char *code) {
    int pc = 0;
    int n = strlen(code);
    while (pc < n) {
        Operation(code, &pc);
        pc++;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Pass Arguments\nfollow exec filename.txt\n");
        return EXIT_FAILURE;
    }

    FILE *fptr;
    const char *filename = argv[1];
    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    fseek(fptr, 0, SEEK_END);
    long file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *code = malloc(file_size + 1);

    if (code == NULL) {
        fprintf(stderr, "Memory allocation error: %s\n", strerror(errno));
        fclose(fptr);
        return EXIT_FAILURE;
    }

    size_t read_size = fread(code, 1, file_size, fptr);
    if (read_size != file_size) {
        fprintf(stderr, "Error reading file: %s\n", strerror(errno));
        free(code);
        fclose(fptr);
        return EXIT_FAILURE;
    }
    code[file_size] = '\0';
    read(code);
    free(code);
    fclose(fptr);

    return EXIT_SUCCESS;
}
