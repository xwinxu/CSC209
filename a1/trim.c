#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Reads a trace file produced by valgrind and an address marker file produced
 * by the program being traced. Outputs only the memory reference lines in
 * between the two markers
 */

int main(int argc, char **argv) {
    
    if(argc != 3) {
         fprintf(stderr, "Usage: %s tracefile markerfile\n", argv[0]);
         exit(1);
    }

    FILE* trace_fp;
    //2 hexadecimal numbers on a single line
    FILE* marker_fp;
    
    //open the file
    trace_fp = fopen(argv[1], "r");
    marker_fp = fopen(argv[2], "r");

    // Addresses should be stored in unsigned long variables
    // unsigned long start_marker, end_marker;
    unsigned long start_marker, end_marker;

    fscanf(marker_fp, "%lx %lx", &start_marker, &end_marker);
    fclose(marker_fp);

    char isml;
    unsigned long hex;
    int bytesize;
    // boolean flag for whether we stop or keep printing
    int found = 0;
    while (fscanf(trace_fp, " %c %lx,%d", &isml, &hex, &bytesize) != EOF) {
       if (hex == start_marker) {
           found = 1;
       } else if (hex == end_marker) {
           found = 0;
       } else {
           if (found) {
               printf("%c,%#lx\n", isml, hex);
           }
       } 
    }
    fclose(trace_fp);

    /* For printing output, use this exact formatting string where the
     * first conversion is for the type of memory reference, and the second
     * is the address
     */
    // printf("%c,%#lx\n", VARIABLES TO PRINT GO HERE);

    return 0;
}
