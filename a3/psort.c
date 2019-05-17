#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "helper.h"

/////////////////// Helper Methods ///////////////////

/* sentinel value to place-hold if dry pipe */
struct rec sentinel = {-1, ""};

/* Wrapper for close */
void Close(int fd, char *location) {
    if (close(fd) == -1) {
        char text[100];
        strcpy(text, "close for ");
        strcat(text, location);
        perror(text);
        exit(1);
    }
}

/* Wrapper for fork system call */
int Fork(void) {
    int res = fork();
    if(res == -1) {
        perror("fork");
        exit(1);
    }
    return res;
}

/* Wrapper for read */
void Read(int fd, void *buf, size_t size) {
    if (read(fd, buf, size) == -1) {
        perror("read");
        exit(1);
    }
}

/* Wrapper for malloc */
void *Malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        perror("malloc");
        exit(1);
    }
    return ptr;
}

/* Wrapper for pipes */
void Pipe(int *fd) {
    if(pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }
}

/* Wrapper for opening files */
FILE *FOpen(char *Input, char *mode) {
    FILE *file = fopen(Input, mode);
    if(file == NULL) {
        perror("error opening file");
        exit(1);
    }
    return file;
}

/* Wrapper for closing files */
int FClose(FILE *fp) {
    if(fclose(fp) == EOF) {
        perror("error closing file");
        exit(1);
    }
    return 0;
}

/* Wrapper for wait. */
int Wait(int *status) {
    int res;
    if((res = wait(status)) == -1) {
        perror("wait");
        exit(1);
    }
    return res;
}

/* Wrapper for write */
size_t FWrite(const void *buffer, size_t rec, size_t count, FILE *output) {
    size_t stat;
    if((stat = fwrite(buffer, rec, count, output)) < 0) {
        perror("write");
        exit(1);
    }
    return stat;
}

//////////////// Program Methods //////////////////

/*
* Adjusts the number of processes accordingly if given faulty input
*/
void adjustParams(int *numProc, int numRec) {
    if (*numProc <= 0) {
        *numProc = 1;
    }
    if (numRec < *numProc) {
        *numProc = numRec;
    }
}

/**
 * Get the smallest output's index to write from the candidates list
 * take from each of the child processes.
 */
int getSmallest(struct rec *candidates, int numProc) {
    int largest;
    int idx;
    for (int i = 0; i < numProc; i++) {
        // only assign largest if not a sentinel value
        if (candidates[i].freq >= 0) {
            largest = candidates[i].freq;
            idx = i;
            break;
        }
    }
    for (int i = 0; i < numProc; i++) {
        if (candidates[i].freq < 0) {
            continue;
        }
        if (candidates[i].freq < largest && candidates[i].freq >= 0) {
            largest = candidates[i].freq;
            idx = i;
        }
    }
    return idx;
}

/**
 * To perform the merging of the data from pipes given from child processes.
 */
void merge(FILE *output, struct rec *candidates, int numProc, int numRec, int pipe_fd[][2]) {
    for(int p = 0; p < numProc; p++) {
        Read(pipe_fd[p][0], &candidates[p], sizeof(struct rec));
    }
    int count = 0;
    int small_idx = 0;
    while (count < numRec) {
        small_idx = getSmallest(candidates, numProc);
        FWrite(&candidates[small_idx], sizeof(struct rec), 1, output);
        count++;
        if (read(pipe_fd[small_idx][0], &candidates[small_idx], sizeof(struct rec)) == 0) {
            candidates[small_idx] = sentinel;
        }
    }
}


/**
 * Write the sorted arrray of struct records to the pipe
 */
void pipeInData(struct rec *sortedRecs, int pipe_fd, int size) {
    for(int i = 0; i < size; i++) {
        if (write(pipe_fd, &sortedRecs[i], sizeof(struct rec)) == -1) {
            perror("child write");
            exit(1);
        }
    }
}

/**
 * Child's work with reading in file and returning a sorted array.
 * Given a proper seeking location and amount of bytes to read.
 */
struct rec *sortFile(FILE *file, int **breakdown, int i) {
    int *readInfo = breakdown[i];
    struct rec* sortedRecs = Malloc(sizeof(struct rec) * readInfo[1]);
    fseek(file, sizeof(struct rec) * readInfo[0], SEEK_SET);
    fread(sortedRecs, sizeof(struct rec), readInfo[1], file);
    qsort(sortedRecs, readInfo[1], sizeof(struct rec), compare_freq);

    return sortedRecs;
}


/**
 * Creates a 2D array for line splits of each child given
 * the total number of processes and records in the file.
 * Populates array in format: [[start line, # lines to read]]
 */
int **splitEvenly(int numProc, int numRec) {
    int **breakdown = Malloc(sizeof(int *) * numProc);
    if(breakdown) {
        for(int i = 0; i < numProc; i++) {
            breakdown[i] = Malloc(sizeof(int) * 2);
        }
    }
    int lineNo = 0;
    int idx = 0;
    int firstHalf = (numRec / numProc) + 1;
    int secHalf = (numRec / numProc);
    int firstGroup = numRec % numProc;

    // split as evenly as possible
    while(idx < firstGroup) {
        breakdown[idx][0] = lineNo;
        breakdown[idx++][1] = firstHalf;
        lineNo = lineNo + firstHalf;
    }
    while(idx < numProc) {
        breakdown[idx][0] = lineNo;
        breakdown[idx++][1] = secHalf;
        lineNo = lineNo + secHalf;
    }
    return breakdown;
}

/*
Read in the arguments from the command line.
*/
void readArgs(int *argc, char ***argv, char **Input, char **Output, int *numProc) {
    extern char *optarg;
    int opt;
    char *buf;

    while ((opt = getopt(*argc, *argv, "n:f:o:")) != -1) {
        switch (opt) {
        case 'n':
            *numProc = strtol(optarg, &buf, 10);
            if (optarg == buf || *buf != '\0') {
                fprintf(stderr, "strtol failed\n");
                exit(1);
            }
            break;
        case 'f':
            *Input = optarg;
            break;
        case 'o':
            *Output = optarg;
            break;
        default:
            fprintf(stderr, "Usage: psort -n <number of processes> -f <inputfile> -o <outputfile>\n");
            exit(1);
        }
    }
}

/**
 * The main program to sort the binary file in ascending frequency order.
 */
int main(int argc, char **argv) {
    int numProc;
    char *Input = NULL;
    char *Output = NULL;
    int numRec;

    if(argc != 7) {
        fprintf(stderr, "Usage: psort -n <number of processes> -f <inputfile> -o <outputfile>\n");
        exit(1);
    }
    
    readArgs(&argc, &argv, &Input, &Output, &numProc);

    FILE *fileinit = FOpen(Input, "rb");
    numRec = (get_file_size(Input) / sizeof(struct rec));
    fclose(fileinit);

    // handle case: split wouldn't work w/ bad input sizes if need
    adjustParams(&numProc, numRec);

    // global 2d array mapping the roles of each child process
    int **breakdown;
    breakdown = splitEvenly(numProc, numRec);

    int pipe_fd[numProc][2];
    for(int i = 0; i < numProc; i++) {
        Pipe(pipe_fd[i]);
    }

    for(int i = 0; i < numProc; i++) {
        int child = Fork();
        if(child == 0) {
            FILE *file = FOpen(Input, "rb");
            Close(pipe_fd[i][0], "read end of child");

            // close [0] ends before/after child, [1] end after only
            for(int k = 0; k < numProc; k++) {
                if (k == i) continue;
                Close(pipe_fd[k][0], "read end of other children");

                if (k >= i) {
                    Close(pipe_fd[k][1], "write end of other children");
                }
            }

            struct rec *sortedRecs = sortFile(file, breakdown, i);
            pipeInData(sortedRecs, pipe_fd[i][1], breakdown[i][1]);
            Close(pipe_fd[i][1], "write end in child");
            // clean up parent traces
            free(sortedRecs);
            for(int i = 0; i < numProc; i++) free(breakdown[i]);
            free(breakdown);
            FClose(file);
            exit(0);
        } else if(child > 0) {
            Close(pipe_fd[i][1], "write end in parent");
        }
    }

    FILE *output = FOpen(Output, "wb");
    
    struct rec candidates[numProc];
    merge(output, candidates, numProc, numRec, pipe_fd);

    // close all unnecessary pipes
    for(int i = 0; i < numProc; i++) {
        Close(pipe_fd[i][0], "read end of parent");
    }

    // check all processes exited successfully w/ no sys call fails
    int status;
    for(int j = 0; j < numProc; j++) {
        Wait(&status);
        if(!WIFEXITED(status)) {
            fprintf(stderr, "Child terminated abnormally\n");
            return 1;
        } else if (WEXITSTATUS(status) != 0) {
            return 1;
        }
    }

    FClose(output);
    for(int i = 0; i < numProc; i++) free(breakdown[i]);
    free(breakdown);

    return 0;
}
