#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MAXMESGDATA 4096

struct mymesg {
    long mesg_len;
    long mesg_type;
    char mesg_data[MAXMESGDATA];
};

#define MESGHDRSIZE (2 * sizeof(long))

ssize_t mesg_send(int fd, struct mymesg *mptr) {
    return write(fd, mptr, MESGHDRSIZE + mptr->mesg_len);
}

ssize_t mesg_recv(int fd, struct mymesg *mptr) {
    ssize_t n;
    size_t len;
    
    if ((n = read(fd, mptr, MESGHDRSIZE)) == 0) {
        return 0;
    } else if (n != MESGHDRSIZE) {
        printf("Client: error reading message header\n");
        return 0;
    }
    
    if ((len = mptr->mesg_len) > 0) {
        if ((n = read(fd, mptr->mesg_data, len)) != len) {
            printf("Client: error reading message data\n");
            return 0;
        }
    }
    return len;
}

void client(int readfd, int writefd) {
    struct mymesg mesg;
    ssize_t n;
    
    // Get filename from user
    printf("Client: Enter filename: ");
    fgets(mesg.mesg_data, MAXMESGDATA, stdin);
    
    mesg.mesg_len = strlen(mesg.mesg_data);
    if (mesg.mesg_data[mesg.mesg_len - 1] == '\n') {
        mesg.mesg_data[--mesg.mesg_len] = '\0';
    }
    
    mesg.mesg_type = 1; // Request type
    
    // Send filename to server
    if (mesg_send(writefd, &mesg) < 0) {
        perror("Client: error sending request");
        return;
    }
    
    printf("Client: Request sent, waiting for response...\n");
    
    // Receive and process responses
    while ((n = mesg_recv(readfd, &mesg)) >= 0) {
        if (n == 0) {
            break; // End of transmission
        }
        
        if (mesg.mesg_type == 1) {
            // File data
            write(STDOUT_FILENO, mesg.mesg_data, mesg.mesg_len);
        } else if (mesg.mesg_type == 2) {
            // Error message
            write(STDOUT_FILENO, mesg.mesg_data, mesg.mesg_len);
            break;
        } else if (mesg.mesg_type == 3) {
            // End of transmission
            break;
        }
    }
    printf("\nClient: Finished receiving data\n");
}

int main(int argc, char **argv) {
    int readfd, writefd;
    
    // Open FIFOs (server should create them first)
    if ((writefd = open(FIFO1, O_WRONLY)) < 0) {
        perror("Client: can not open FIFO1 for write");
        exit(1);
    }
    printf("Client: FIFO1 opened for write\n");
    
    if ((readfd = open(FIFO2, O_RDONLY)) < 0) {
        perror("Client: can not open FIFO2 for read");
        exit(1);
    }
    printf("Client: FIFO2 opened for read\n");
    
    client(readfd, writefd);
    
    close(readfd);
    close(writefd);
    
    printf("Client: terminated!\n");
    exit(0);
}
