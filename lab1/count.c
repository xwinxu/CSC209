/*
 * This program takes in one command-line argument n, which must be a
 * non-negative integer.
 * It then prints the first n non-negative integers (from 0 to n-1).
 * Note that printf doesn't automatically insert a newline character,
 * so all the integers are printed consecutively.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    // checks if no arguments are passed, since the first argument is always the name of the program
    // can probably also check for < some num of args if program requires certain number
    if (argc == 1) {
        printf("Usage: %s NUM\n", argv[0]);
        // tells environment that program did not succeed
        return 1;
    }

    // Convert the first argument to an integer.
    int n = strtol(argv[1], NULL, 10);

    for (int i = 0; i < n; i++) {
        printf("%d", i);
    }
    return 0;
}
