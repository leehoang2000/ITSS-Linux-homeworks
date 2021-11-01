#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define PIPE_READ_WRITE 2
#define STD_IN 0
#define STD_OUT 1
#define BUFSIZE 80

// main process
int main()
{
    printf("main    :start\n");
    pid_t pid = 0; // Process ID
    int pipe_c2p[PIPE_READ_WRITE] = {0}; // child to parent
    char buf[BUFSIZE] = {'\0'};
    int readSize = 0;

    // create pipe
    if(pipe(pipe_c2p) == -1)
    {
        perror("process Generate pipe");
        exit(1);
    }

    // create child process
    switch (pid = fork())
    {
        case -1:
        {
            perror("process Generate fork");
            // close file descriptors for input/output
            close(pipe_c2p[PIPE_READ]);
            close(pipe_c2p[PIPE_WRITE]);
            exit(1);
        }
        case 0: // child process
        {
            printf("childProcess start\n");
            // close file descriptors for input
            close(pipe_c2p[PIPE_READ]);
            strcpy(buf,"written string\n");
            write(pipe_c2p[PIPE_WRITE], buf, strlen(buf)+1); // +1 for null char
            memset(buf, 0, sizeof(buf));
            // close file descriptors for output
            close(pipe_c2p[PIPE_WRITE]);
            printf("childProcess end\n");
            exit(0);
        }
        default: // parent process
        {
            printf("parentProcess start\n");
            // close file descriptors for output
            close(pipe_c2p[PIPE_WRITE]);
            while(1)
            {
                if((readSize = read(pipe_c2p[PIPE_READ], buf, BUFSIZE))==0)
                {
                    break; // If end of file
                }
                // write(STD_OUT, buf, readSize);
                printf("Read: %s",buf);
            }
            // close file descriptors for input
            close(pipe_c2p[PIPE_READ]);
            printf("parentProcess end\n");
            break;
        }
    }
    printf("main    :end\n");
    return 0;
}