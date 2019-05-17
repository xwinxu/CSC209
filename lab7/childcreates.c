#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<signal.h>

int main(int argc, char **argv) {
    int i;
    int iterations;

    if (argc != 2) {
        fprintf(stderr, "Usage: forkloop <iterations>\n");
        exit(1);
    }

    iterations = strtol(argv[1], NULL, 10);

    // so that each process creates exactly one new process
    int childCreated = 0;
    
    // for buffering child exit code to wait
    int stat = 0;

    for (i = 0; i < iterations; i++) {
        if(childCreated == 0) {
            childCreated = fork(); // only the first loop iteration would change the childCreated flag
            
            int n = childCreated;
            if (n < 0) {
                perror("fork");
                exit(1);
            }
            // waiting for child process
           if (n > 0) {
               wait(&stat);
           }
        }
        // pid is process id, parent id is ppid
        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
    }

    return 0;
}
