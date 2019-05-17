#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "family.h"

/* Number of word pointers allocated for a new family.
   This is also the number of word pointers added to a family
   using realloc, when the family is full.
*/
static int family_increment = 0;


/* Set family_increment to size, and initialize random number generator.
   The random number generator is used to select a random word from a family.
   This function should be called exactly once, on startup.
*/
void init_family(int size) {
    family_increment = size;
    srand(time(0));
}


/* Given a pointer to the head of a linked list of Family nodes,
   print each family's signature and words.

   Do not modify this function. It will be used for marking.
*/
void print_families(Family* fam_list) {
    int i;
    Family *fam = fam_list;
    
    while (fam) {
        printf("***Family signature: %s Num words: %d\n",
               fam->signature, fam->num_words);
        for(i = 0; i < fam->num_words; i++) {
            printf("     %s\n", fam->word_ptrs[i]);
        }
        printf("\n");
        fam = fam->next;
    }
}

/////////// Helper Functions /////////////////
/* 
    Malloc wrapper to check for error
*/
void *Malloc(size_t size, char *address) {
    void *result = malloc(size);
    if (result == NULL) {
        perror(address);
        exit(1);
    }
    return result;
}


/*
    Helper function for generating signature for a word string.
*/
char *generate_sig(char *word, char letter) {
   char *sig = Malloc((strlen(word) + 1) * sizeof(char), "malloc signature");
   memset(sig, 0, (strlen(word) + 1) * sizeof(char));
   int idx = 0;
   while (*word != '\0')
   {
       if (*word == letter)
       {
           sig[idx++] = letter;
       }
       else
       {
           sig[idx++] = '-';
       }
       word++;
   }
   sig[idx] = '\0';
   return sig;
}



//////////////// Assignment Functions //////////////////

/*8 Return a pointer to a new family whose signature is 
   a copy of str. Initialize word_ptrs to point to 
   family_increment+1 pointers, numwords to 0, 
   maxwords to family_increment, and next to NULL.
*/
Family *new_family(char *str) {
    Family *fam_ptr = Malloc(sizeof(Family), "malloc for new fam member");
    fam_ptr->signature = Malloc((strlen(str) + 1) * sizeof(char), "malloc fam sig");
    strcpy(fam_ptr->signature, str);
    fam_ptr->num_words = 0;
    fam_ptr->max_words = family_increment;
    fam_ptr->word_ptrs = Malloc((family_increment+1) * sizeof(char *), "malloc for new fam's word ptrs");
    // as per the diagram, must have a NULL to terminate
    // when we add, we add at the num_words index, which would overwrite this null here
    fam_ptr->word_ptrs[0] = NULL;
    fam_ptr->next = NULL;

    return fam_ptr;
}


/* Add word to the next free slot fam->word_ptrs.
   If fam->word_ptrs is full, first use realloc to allocate family_increment
   more pointers and then add the new pointer.
*/
void add_word_to_family(Family *fam, char *word) {
    // if word_ptrs is full
    if(fam->num_words >= fam->max_words) {
        // increment max_words to old + fam_increment
        fam->max_words += family_increment;
        // +1 for the NULL at term
        fam->word_ptrs = realloc(fam->word_ptrs, (fam->max_words + 1) * sizeof(char *));
    }
    // realloc error checking
    if(fam->word_ptrs == NULL) {
        perror("realloc add word to fam");
        exit(1);
    }
    // add new pointer after (reallocating if necessary), THEN incrementing the word
    fam->word_ptrs[fam->num_words++] = word;
    fam->word_ptrs[fam->num_words] = NULL;
}


/* Return a pointer to the family whose signature is sig;
   if there is no such family, return NULL.
   fam_list is a pointer to the head of a list of Family nodes.
*/
Family *find_family(Family *fam_list, char *sig) {
    // Family *fam_list_ptr = fam_list;
    while (fam_list != NULL) {
        // we know all the signatures will be a certain length (specified by the user)
        if (strcmp(fam_list->signature, sig) == 0) {
            return fam_list;
        }
        fam_list = fam_list->next;
    }
    return NULL;
}


/* Return a pointer to the family in the list with the most words;
   if the list is empty, return NULL. If multiple families have the most words,
   return a pointer to any of them.
   fam_list is a pointer to the head of a list of Family nodes.
*/
Family *find_biggest_family(Family *fam_list) {
    Family *max_ptr = fam_list;
    int maxwords = fam_list->num_words;
    
    // check if list is a null pointer
    if(fam_list == NULL) {
        return NULL;
    }
    
    // given that the list has at least one fam member...
    while(fam_list != NULL) {
        if(fam_list->num_words > maxwords) {
            maxwords = fam_list->num_words;
            max_ptr = fam_list;
        }
        fam_list = fam_list->next;
    }
    return max_ptr;
}


/* Deallocate all memory rooted in the List pointed to by fam_list. */
void deallocate_families(Family *fam_list) {
    Family *fl_ptr = fam_list;
    Family *ptr_temp;
    while (fl_ptr) {
        free(fl_ptr->signature);
        free(fl_ptr->word_ptrs);
        ptr_temp = fl_ptr;
        fl_ptr = fl_ptr->next;
        free(ptr_temp);
    }
}


/* Generate and return a linked list of all families using words pointed to
   by word_list, using letter to partition the words.

   Implementation tips: To decide the family in which each word belongs, you
   will need to generate the signature of each word. Create only the families
   that have at least one word from the current word_list.
*/

Family *generate_families(char **word_list, char letter) {
    Family *famhead = NULL;
    // pointer to the front of the pruned word list
    char **wl_ptr = word_list;
    Family *fam_found;

    // as long as we're at a word and not at the end
    while (*wl_ptr) {
        // generate a signature for the word
        char *sig = generate_sig(*wl_ptr, letter);
        // check to see if such a family exists already
        fam_found = find_family(famhead, sig);
        if (fam_found == NULL){
            Family *new_mem = new_family(sig);
            new_mem->next = famhead;
            // print new_mem next
            famhead = new_mem;
            // make it not null anymore
            fam_found = new_mem;
        }
        add_word_to_family(fam_found, *wl_ptr);
        wl_ptr++;
        free(sig); //we deallocate families later
    }
    return famhead;
}

/* Return the signature of the family pointed to by fam. */
char *get_family_signature(Family *fam) {
    return fam->signature;
}


/* Return a pointer to word pointers, each of which
   points to a word in fam. These pointers should not be the same
   as those used by fam->word_ptrs (i.e. they should be independently malloc'd),
   because fam->word_ptrs can move during a realloc.
   As with fam->word_ptrs, the final pointer should be NULL.

   fam is a linked list of family members.
*/
char **get_new_word_list(Family *fam) {
    // create a copy of the word list of fam and return it
    char **pt_to_wd_ptrs = Malloc((fam->num_words + 1) * sizeof(char*), "malloc for copy of fam's wdptrs");
    int idx = 0;
    char **fam_wd_ptr = fam->word_ptrs;
    while (*fam_wd_ptr) {
        pt_to_wd_ptrs[idx++] = *fam_wd_ptr;
        fam_wd_ptr++;
    }
    pt_to_wd_ptrs[idx] = NULL;
    return pt_to_wd_ptrs;
}


/* Return a pointer to a random word from fam. 
   Use rand (man 3 rand) to generate random integers.
*/
char *get_random_word_from_family(Family *fam) {
    // random int between 0 and (fam->num_words - 1)
    int rand_var = rand() % fam->num_words;
    return (fam->word_ptrs)[rand_var];
}
