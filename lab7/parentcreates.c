#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int i;
    int iterations;

    if (argc != 2) {
        fprintf(stderr, "Usage: forkloop <iterations>\n");
        exit(1);
    }

    iterations = strtol(argv[1], NULL, 10);
    // keep track of the original PID, overall parent
    int originalPID = getpid();
    
    // for child process
    int stat = 0;

    for (i = 0; i < iterations; i++) {
        if(getpid() == originalPID) {
            int n = fork();
            if (n < 0) {
                perror("fork");
                exit(1);
            }
           if (n > 0) {
               wait(&stat);
           }
        }
        // pid is process id, parent id is ppid
        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
    }

    return 0;
}
