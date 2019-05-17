#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Write the copy function to perform exactly as strncpy does, with one
   exception: your copy function will guarantee that dest is always
   null-terminated.
   You shoud read the man page to learn how strncpy works.

  NOTE: You must write this function without using any string functions.
  The only function that should depend on string.h is memset.
 */

char *copy(char *dest, const char *src, int capacity) {
    int idx = 0;
    // don't do int idx here, will reassign the variable
    for(idx = 0; idx < capacity && src[idx] != '\0'; idx++) {
        // could also just assign: dest[idx] = src[idx]
        dest[idx] = src[idx];
        // *(dest + idx) != *(dest[idx]) here you get the integer address
        // memset(&(dest[idx]), src[idx], 1);
    }

    //while(idx <= capacity) strict less because we want last char to be null terminated
    while(idx < capacity) {
        dest[idx++] = '\0';
        // memset(&(dest[idx++]), '\0', 1);
    }

    if(capacity > 0) dest[capacity - 1] = '\0';
    // memset(&(dest[capacity - 1]), '\0', 1);
    
    // while(term != 1) {
    //     if(src[idx] == '\0' || idx >= capacity || idx >= (strlen(src) + 1)) {
    //     // character is a null terminator
    //         term = 1;
    //     } else {
    //         memset(&(dest[idx]), src[idx], 1);
    //         idx++;
    //     }
    // }
    // set last character to null terminator
    return dest;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: copy size src\n");
        exit(1);
    }
    int size = strtol(argv[1], NULL, 10);
    char *src = argv[2];

    char dest[size];
    memset(dest, 'x', size);

    copy(dest, src, size);

    printf("%s\n", dest);
    return 0;
}
