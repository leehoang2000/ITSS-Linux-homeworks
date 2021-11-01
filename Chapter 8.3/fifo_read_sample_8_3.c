// Only reads from FIFO

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define FIFO_FILE_PATH "fifofile" // Path of FIFO file to read from
#define BUFSIZE 256

#define STD_IN 0
#define STD_OUT 1

int main()
{
    int i, fifoFd, size;
    char buf[BUFSIZE];

    // Open for reading only
    if( (fifoFd = open(FIFO_FILE_PATH, O_RDONLY)) == -1)
    {
        perror("fifofile open");
        exit(1);
    }

    // Output to FIFO file
    while( (size = read(fifoFd, buf, BUFSIZE)) != 0)
    {
        write(STD_OUT, buf, size);
    }

    close(fifoFd);

    return 0;
}