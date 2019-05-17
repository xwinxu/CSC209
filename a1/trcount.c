#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Constants that determine that address ranges of different memory regions

#define GLOBALS_START 0x400000
#define GLOBALS_END   0x700000
#define HEAP_START   0x4000000
#define HEAP_END     0x8000000
#define STACK_START 0xfff000000

int main(int argc, char **argv) {
    
    FILE *fp = NULL;

    // contains no arguments
    if(argc == 1) {
        fp = stdin;

    // one argument, aka the trace file
    } else if(argc == 2) {
        fp = fopen(argv[1], "r");
        if(fp == NULL){
            perror("fopen");
            exit(1);
        }
    } else {
        fprintf(stderr, "Usage: %s [tracefile]\n", argv[0]);
        exit(1);
    }

    /* Complete the implementation */
    // program should only count data references (Load, Store, Modify)

    /* Use these print statements to print the ouput. It is important that 
     * the output match precisely for testing purposes.
     * Fill in the relevant variables in each print statement.
     * The print statements are commented out so that the program compiles.  
     * Uncomment them as you get each piece working.
     */
 
     // initialized counter variables
    // only count data references (load, store modify)
    int instruct, mod, loads, stores, globals, heap, stack;
    instruct = mod = loads = stores = globals = heap = stack = 0;

    char isml;
    unsigned long hex;
    
    //\r\n for old move to front of line deal
    while (fscanf(fp, "%c,%lx\n", &isml, &hex) != EOF) {
        // if (strncmp(&isml, "I", 1) == 0) {
        //     instruct += 1;
        // }
        // else if (strncmp(&isml, "M", 1) == 0) {
        //     mod += 1;
        // }
        // else if (strncmp(&isml, "L", 1) == 0) {
        //     loads += 1;
        // }
        // else {
        //     stores += 1;
        // }

        if (isml == 'I') {
            instruct += 1;
        } else if (isml == 'M') {
            mod += 1;
        } else if (isml == 'L') {
            loads += 1;
        } else {
            stores += 1;
        }

        // if (strncmp(&isml, "I", 1) != 0)
        if (isml != 'I') {
            if (GLOBALS_START <= hex && hex <= GLOBALS_END) {
                globals += 1;
            } else if (HEAP_START <= hex && hex <= HEAP_END) {
                heap += 1;
            } else if (STACK_START <= hex) {
                    stack += 1;
            }
        }
    }

    printf("Reference Counts by Type:\n");
    printf("    Instructions: %d\n", instruct);
    printf("    Modifications: %d\n", mod);
    printf("    Loads: %d\n", loads);
    printf("    Stores: %d\n", stores);
    printf("Data Reference Counts by Location:\n");
    printf("    Globals: %d\n", globals);
    printf("    Heap: %d\n", heap);
    printf("    Stack: %d\n", stack);

    return 0;
}
