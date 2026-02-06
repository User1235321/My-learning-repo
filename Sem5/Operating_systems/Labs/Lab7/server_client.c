#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLINE 1024

void client(int readfd, int writefd) {
    size_t len;
    ssize_t n;
    char buff[MAXLINE];
    
    printf("Client: Enter filename: ");
    fgets(buff, MAXLINE, stdin);
    
    len = strlen(buff);
    if (buff[len-1] == '\n')
        len--;
    
    write(writefd, buff, len);
    
    while ((n = read(readfd, buff, MAXLINE)) > 0) {
        write(STDOUT_FILENO, buff, n);
    }
}

void server(int readfd, int writefd) {
    int fd;
    ssize_t n;
    char buff[MAXLINE + 1];
    char error_msg[] = "Error: Cannot open file\n";
    
    if ((n = read(readfd, buff, MAXLINE)) == 0) {
        printf("Server: End of file when reading pathname\n");
        exit(1);
    }
    buff[n] = '\0';
    
    printf("Server: Trying to open file: %s\n", buff);
    
    if ((fd = open(buff, O_RDONLY)) < 0) {
        printf("Server: Cannot open file %s\n", buff);
        write(writefd, error_msg, strlen(error_msg));
    } else {
        printf("Server: File opened successfully\n");
        while ((n = read(fd, buff, MAXLINE)) > 0) {
            write(writefd, buff, n);
        }
        close(fd);
    }
}

int main(int argc, char **argv) {
    int pipe1[2], pipe2[2];
    pid_t childpid;
    
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("pipe error");
        exit(1);
    }
    
    if ((childpid = fork()) == 0) {
        // Child process - server
        close(pipe1[1]);
        close(pipe2[0]);
        server(pipe1[0], pipe2[1]);
        exit(0);
    }
    
    // Parent process - client
    close(pipe1[0]);
    close(pipe2[1]);
    client(pipe2[0], pipe1[1]);
    
    waitpid(childpid, NULL, 0);
    exit(0);
}
