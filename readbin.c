#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include "helper.h"


// unsigned char buffer[10];

/*
Run with gcc -Wall -g -o readbin readbin.c 
./readbin 8-words.b
*/

void read_binary_data(FILE *ptr, FILE *output) {
    struct rec record;
    int count = 0;
    // read in to record, our buffer
    while( fread(&record, sizeof(struct rec), 1, ptr) == 1) {
        printf("word: %s freq: %d\n", record.word, record.freq);
        fprintf(output, "word: %s freq: %d\n", record.word, record.freq);
        count++;
    }
    printf("total line: %d\n", count);
}

int main(int argc, char **argv) {
    // 8_words.b
    FILE *file = fopen(argv[1], "rb");
    if(file == NULL) {
        fprintf(stderr, "Cannot open binary file\n");
    }
    FILE *output = fopen(argv[2], "w+");
    if(output == NULL) {
        fprintf(stderr, "Cannot write to file\n");
    }
    read_binary_data(file, output);

    return 0;
}