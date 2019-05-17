#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "family.h"
#include "reading.h"

/////////// tester functions ////////////
    void test_new_family() {
        char *str = "--e-";
        Family *new_fam = new_family(str);
        print_families(new_fam);
    }

    void test_add_word_to_family() {
        char *str = "--e-";
        Family *new_fam = new_family(str);
        char *word1 = "heel";
        char *word2 = "pree";

        add_word_to_family(new_fam, word1);
        add_word_to_family(new_fam, word2);
        print_families(new_fam);
    }

    void test_find_family(Family * new_fam1) {
        Family *resultA = find_family(new_fam1, "-m-m");
        Family *resultB = find_family(new_fam1, "-m--");
        printf("%s\n", resultA->signature);
        printf("%d\n", resultB == NULL);
    }

    void test_find_biggest_family(Family * new_fam1) {
        print_families(find_biggest_family(new_fam1));
    }

    void test_generate_families() {
        char **words = read_words("dictionary.txt");
        words[10] = NULL; //Take first 10 words
        Family *fam_generated = generate_families(words, 'a');
        print_families(fam_generated);
    }

    int main()
    {
        char *str1 = "--e-";
        Family *new_fam1 = new_family(str1);
        char *str2 = "-m-m";
        Family *new_fam2 = new_family(str2);
        char *str3 = "n--";
        Family *new_fam3 = new_family(str3);

        // create the linked list
        new_fam2->next = new_fam3;
        new_fam1->next = new_fam2;

        add_word_to_family(new_fam2, "amam");
        printf("%s\n", "added word amam");
        add_word_to_family(new_fam2, "bmam");
        printf("%s\n", "added word bmam");
        add_word_to_family(new_fam3, "neee");
        printf("%s\n", "added word neee");

        test_new_family();
        printf("---------%s----------\n", "new fam");
        test_add_word_to_family();
        printf("----------%s---------\n", "add word");
        test_find_family(new_fam1);
        printf("----------%s---------\n", "find fam");
        test_find_biggest_family(new_fam1);
        printf("---------%s----------\n", "biggest fam");
        test_generate_families();
        printf("---------%s----------\n", "generate fam");

        return 0;
}
