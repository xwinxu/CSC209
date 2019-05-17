#include <stdio.h>

int main() {
    char phone[11];
    int input;
    int err = 0;

    scanf("%s", phone);
    
    while (scanf("%d", &input) != EOF) {
        if (input == -1) {
            printf("%s\n", phone);
        } else if (input >=0 && input <= 9) {
            printf("%c\n", phone[input]);
        } else {
            printf("ERROR\n");
            err = 1;
        }
    }
    return err;
}
