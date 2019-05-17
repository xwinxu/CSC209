#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifndef PORT
  #define PORT 30000
#endif
#define BUF_SIZE 128

int send_stdin(int sock_fd) {
    // Prompt user for a username
    char username[BUF_SIZE + 1];
    int nbytes;
    if((nbytes = read(STDIN_FILENO, username, BUF_SIZE)) < 0) {
        perror("client read username");
        exit(1);
    }
    // null terminate the string read
    username[nbytes] = '\0';

    // Send username to the server
    int nwrite = write(sock_fd, username, nbytes);
    if(nwrite != nbytes) {
        perror("client: write user");
        close(sock_fd);
        exit(1);
    }

    return 0;
}

int main(void) {

    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }

    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 1) {
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }

    send_stdin(sock_fd);

    // Read input from the user, send it to the server, and then accept the
    // echo that returns. Exit when stdin is closed.
    char buf[BUF_SIZE + 1];

    // fd_set to monitor 2 file descriptors: socket w/ server and stdin
    int max_fd = STDIN_FILENO;
    if(sock_fd > max_fd) max_fd = sock_fd;
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(sock_fd, &fds);
    FD_SET(STDIN_FILENO, &fds);
    //int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;
    //max_fd++;

    while (1) {
        // Make a copy to select from
        fd_set listen_fds = fds;
        int nready = select(max_fd + 1, &listen_fds, NULL, NULL, NULL);
        if(nready == -1) {
            perror("client: select");
            exit(1);
        }

        if(FD_ISSET(sock_fd, &listen_fds)) {
            int num_read = read(sock_fd, buf, BUF_SIZE);
            buf[num_read] = '\0';
            printf("Received from server: %s", buf);
        } 
        if(FD_ISSET(STDIN_FILENO, &listen_fds)) {
            send_stdin(sock_fd);
      //     int num_read = read(STDIN_FILENO, buf, BUF_SIZE);
      //      if (num_read == 0) {
      //          break;
      //      }
      //      buf[num_read] = '\0';         

      //      int num_written = write(sock_fd, buf, num_read);
      //      if (num_written != num_read) {
      //          perror("client: write");
      //          close(sock_fd);
      //          exit(1);
      //      }
        }
    }

    close(sock_fd);
    return 0;
}
