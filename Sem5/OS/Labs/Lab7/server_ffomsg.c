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
        printf("Server: error reading message header\n");
        return 0;
    }
    
    if ((len = mptr->mesg_len) > 0) {
        if ((n = read(fd, mptr->mesg_data, len)) != len) {
            printf("Server: error reading message data\n");
            return 0;
        }
    }
    return len;
}

void server(int readfd, int writefd) {
    FILE *fp;
    ssize_t n;
    struct mymesg mesg;
    char error_msg[] = "Error: Cannot open file\n";
    
    // Read filename from client
    if (mesg_recv(readfd, &mesg) == 0) {
        printf("Server: End of file when reading filename\n");
        return;
    }
    mesg.mesg_data[mesg.mesg_len] = '\0';
    
    printf("Server: Received request for file: %s\n", mesg.mesg_data);
    
    // Try to open file
    if ((fp = fopen(mesg.mesg_data, "r")) == NULL) {
        // Send error message
        mesg.mesg_type = 2;
        strcpy(mesg.mesg_data, error_msg);
        mesg.mesg_len = strlen(error_msg);
        mesg_send(writefd, &mesg);
    } else {
        // Send file content
        mesg.mesg_type = 1;
        while ((n = fread(mesg.mesg_data, 1, MAXMESGDATA, fp)) > 0) {
            mesg.mesg_len = n;
            mesg_send(writefd, &mesg);
        }
        fclose(fp);
        
        // Send end of transmission message
        mesg.mesg_type = 3;
        mesg.mesg_len = 0;
        mesg_send(writefd, &mesg);
    }
}

int main(int argc, char **argv) {
    int readfd, writefd;
    
    // Create FIFOs
    if (mknod(FIFO1, S_IFIFO | 0666, 0) < 0) {
        perror("Server: can not create FIFO1");
        exit(1);
    }
    printf("Server: FIFO1 created!\n");
    
    if (mknod(FIFO2, S_IFIFO | 0666, 0) < 0) {
        unlink(FIFO1);
        perror("Server: can not create FIFO2");
        exit(1);
    }
    printf("Server: FIFO2 created!\n");
    
    // Open FIFOs
    if ((readfd = open(FIFO1, O_RDONLY)) < 0) {
        perror("Server: can not open FIFO1 for read");
        exit(1);
    }
    printf("Server: FIFO1 opened for read\n");
    
    if ((writefd = open(FIFO2, O_WRONLY)) < 0) {
        perror("Server: can not open FIFO2 for write");
        exit(1);
    }
    printf("Server: FIFO2 opened for write\n");
    
    server(readfd, writefd);
    
    close(readfd);
    close(writefd);
    
    // Cleanup FIFOs
    unlink(FIFO1);
    unlink(FIFO2);
    
    printf("Server: terminated!\n");
    exit(0);
}
