#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int ret;

    printf("A\n");

    ret = fork();

    printf("B\n");

    if (ret < 0) {
        perror("fork");
        exit(1);
    } else if (ret == 0) {
        printf("C\n");
    } else {
        printf("D\n");
    }

    printf("E\n");
    return 0;
}

/*
Q1: lines of output printed more than once
    - B: 2x
    - C: 1x
    - D: D
    - E: 2x

Q2: All different possible orders for the output
    - A, B, B, C, D, E, E
    - A, B, D, E, B, C, E if child runs after parent
    - ??? Does parent go first then child, do their timings intersect
*/