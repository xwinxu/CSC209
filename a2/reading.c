#include "reading.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Read all words from filename and return them in a 2D array. */
char **read_words(char *filename) {
    char buffer[MAX_WORD_LENGTH + 1];
    FILE *fp;
    int word_count;
    char **words;

    fp = fopen(filename, "r");
    if (!fp) {
      perror("fopen");
      exit(1);
    }

    word_count = 0;
    // the number of words to store in array, is *words
    words = malloc(MAX_WORDS * sizeof(*words));
    if (words == NULL) {
        perror("malloc");
        exit(1);
    }

    /*Get and store all words*/
    // reads a line from stream fp, and stores it into the string pointed to by buffer, a char*
    while (fgets(buffer, MAX_WORD_LENGTH, fp)) {
        if(buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0'; /*Delete newline*/
        }
        // get length of the buffer and add 1 more for the null terminator
        words[word_count] = malloc(strlen(buffer) + 1);
        if (words[word_count] == NULL) {
            perror("malloc");
            exit(1);
        }

        /* strcpy is safe because we just allocated exactly enough space. */
        // copy string pointed to by buffer to destination words[word_count]
        // words[word_count] is a pointer to first elem of array
        strcpy(words[word_count], buffer);
        word_count++;
    }
    words[word_count] = NULL;

    fclose(fp);
    return words;
}

/* Deallocate all memory acquired by read_words. */
void deallocate_words(char **words) {
    // point to the words 2d array memory address
    char **p = words;
    // while the array is not empty
    while(*p) {
        free(*p);
        p++;
    }
    free(words);
}
