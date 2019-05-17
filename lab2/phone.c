#include <stdio.h>

// read two values from standard input, 1st 10 char string and 2nd is an integer
// No command line arguments

int main() {
    // need to allocate space for 11 characters
    char phone[11];
    int value;
    scanf("%s %d", phone, &value);

    if (value == -1) {
        printf("%s\n", phone);
        return 0;
    } else if (value <= 9 && value >= 0) {
       printf("%c\n", phone[value]);
       return 0;
    } else {
        printf("ERROR\n");
        return 1;
    }
}
