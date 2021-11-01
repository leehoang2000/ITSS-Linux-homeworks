#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define FIFO_FILE_PATH "fifofile" // Path of FIFO file to write to
#define READ_FILE_PATH "readfile" // Path to read from
#define BUFSIZE 256

int main()
{
    int i, fifoFd, readFd, size;
    char buf[BUFSIZE];
    // Make FIFO file
    if(mkfifo(FIFO_FILE_PATH,0666) == -1)
    {
        perror("mkfifo");
    }
    // Open for writing only
    if( (fifoFd = open(FIFO_FILE_PATH, O_WRONLY)) == -1)
    {
        perror("fifofile open");
        exit(1);
    }
    // Read file
    if( (readFd = open(READ_FILE_PATH, O_RDONLY)) == -1)
    {
        perror("readfile open");
        exit(1);
    }
    // Output to FIFO file
    while( (size = read(readFd, buf, BUFSIZE)) != 0)
    {
        write(fifoFd, buf, size);
    }
    close(readFd);
    close(fifoFd);
    unlink(FIFO_FILE_PATH);
    return 0;
}