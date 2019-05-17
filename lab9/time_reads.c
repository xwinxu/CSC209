/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Constant for the number of ints in the binary file*/
#define NUM 100

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;

// parameter signal if multiple signals were set by sigaction
void handler(int sig) {
  if(sig == SIGPROF) {
    printf(MESSAGE, num_reads, seconds);
    exit(0);
  }
}

/* The first command-line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }

    //signal handler
    struct sigaction act;

    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    // modify signal handler for SIGPROF
    if (sigaction(SIGPROF, &act, NULL) == -1) {
      perror("sigaction");
      exit(1);
    }

    // set up itimer
    struct itimerval timer;
    timer.it_value.tv_sec = seconds;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0; 
    setitimer(ITIMER_PROF, &timer, NULL);

    /* In an infinite loop, read an int from a random location in the file,
     * and print it to stderr.
     */
    for (;;) {
      int rando = random() % NUM;
      int datum;
      if(fseek(fp, sizeof(int) * rando, SEEK_SET) != 0) perror("fseek");
      fread(&datum, sizeof(int), 1, fp);
      printf("%d\n", datum);
      num_reads++;
    }
    return 1; // something is wrong if we ever get here!
}
