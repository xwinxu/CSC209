#include <stdio.h>
#include <stdlib.h>

/* Return a pointer to an array of two dynamically allocated arrays of ints.
   The first array contains the elements of the input array s that are
   at even indices.  The second array contains the elements of the input
   array s that are at odd indices.

   Do not allocate any more memory than necessary.
*/
int **split_array(const int *s, int length) {
    // pointer to an array of arrays (dyanmically allocated array) of ints
    int** arr_pt = malloc(2 * sizeof(int*));
    // same as *arr_pt[0]
    // even is ceiling, odd is floor
    arr_pt[0] = malloc(((length + 1) / 2) * sizeof(int));
    arr_pt[1] = malloc((length / 2) * sizeof(int));

    for (int i=0; i < length; i++) {
        if (i % 2 == 0) {
            // dividing two ints gives two ints
            arr_pt[0][i / 2] = s[i];
        } else {
            arr_pt[1][i / 2] = s[i];
        }
    }

    return arr_pt;
}

/* Return a pointer to an array of ints with size elements.
   - strs is an array of strings where each element is the string
     representation of an integer.
   - size is the size of the array
 */

int *build_array(char **strs, int size) {
    int* arr_pt = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        arr_pt[i] = strtol(strs[i + 1], NULL, 10);
    }

    return arr_pt;
}


int main(int argc, char **argv) {
    /* Replace the comments in the next two lines with the appropriate
       arguments.  Do not add any additional lines of code to the main
       function or make other changes.
     */
    int *full_array = build_array(argv, argc - 1);
    int **result = split_array(full_array, argc - 1);

    printf("Original array:\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%d ", full_array[i]);
    }
    printf("\n");

    printf("result[0]:\n");
    for (int i = 0; i < argc/2; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    printf("result[1]:\n");
    for (int i = 0; i < (argc - 1)/2; i++) {
        printf("%d ", result[1][i]);
    }
    printf("\n");
    free(full_array);
    free(result[0]);
    free(result[1]);
    free(result);
    return 0;
}
