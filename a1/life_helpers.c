#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Takes character array and size of it. Prints character of array followed by newline.
*/
void print_state(char *state, int size) {
    printf("%s\n", state);
}

/* 
Takes character array and updates the state of the array
*/
void update_state(char *state, int size) {
   // make a copy of the current state
    char state_cp[size];
    // strncpy(state_cp, state, size);
    // state_cp[size-1] = '\0';

    for (int i=0; i < size; i++) {
        state_cp[i] = state[i];
    }

    for (int i=1; i < size - 1; i++) {
        if (state_cp[i-1] == state_cp[i+1]) {
            // modify the original state character array pointer directly
            state[i] = '.';
        } else if (state_cp[i-1] != state_cp[i+1]) {
           state[i] = 'X';
        } 
    }
}
