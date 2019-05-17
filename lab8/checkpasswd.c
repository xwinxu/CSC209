#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
 
  //pipe 
  int pipe_fd[2];
  if(pipe(pipe_fd) == -1) {
      perror("pipe");
      exit(1);
  }
  
  //child fork call
  int val = fork();
  if(val < 0) {
      perror("fork");
      exit(1);
  }
  else if(val == 0) {
      //child
      dup2(pipe_fd[0], fileno(stdin));
      // prompt in parent
      // optional close below after dup2
      if(close(pipe_fd[0]) == -1) {
          perror("close read end for child");
          exit(1);
      }
       if(close(pipe_fd[1]) == -1) {
          perror("close write end for child");
          exit(1);
      }
// want child to read from the stdin previously written by the parent
      // dup2(pipe_fd[0], fileno(stdin)); 
      execl("./validate", "validate", NULL);
      // puts("execl executed"); this is never reached, so no error below
      perror("exec");
      exit(1); 
  }
  //parent stuff
  else if(val > 0) {
    int status;  
      if(close(pipe_fd[0]) == -1) {
            perror("close writing in parent");
            exit(1);
      }
      //write to child
      if(write(pipe_fd[1], user_id, MAX_PASSWORD) == -1) {
          perror("write");
      }
      if(write(pipe_fd[1], password, MAX_PASSWORD) == -1) {
          perror("write");
      }
      if(close(pipe_fd[1]) == -1) {
          perror("close write end of parent");
          exit(1);
      }
      // check exit status
      if(wait(&status) != -1) {
          if(WIFEXITED(status)) {
              if(WEXITSTATUS(status) == 0) {
                  printf(SUCCESS);
              } else if(WEXITSTATUS(status) == 1) {
                  exit(1);
              } else if(WEXITSTATUS(status) == 2) {
                  printf(INVALID);
              } else if(WEXITSTATUS(status) == 3) {
                  printf(NO_USER);
              }
          }
    } else {
        perror("wait");
    }
  }

  return 0;
}
