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

            // ls will write to stdout, redirect to pipe write head
            close(STD_OUT);
            dup(pipe_c2p[PIPE_WRITE]);

            // close file descriptors for input/output
            close(pipe_c2p[PIPE_READ]);
            close(pipe_c2p[PIPE_WRITE]);
            execlp("ls","-l",NULL);
            // errors if not exited
            perror("execlp: ls -l");
            exit(1);
        }
        default: // parent process
        {
            printf("parentProcess start\n");
            // wc -l will read stdin, take pipe read head instead
            close(STD_IN);
            dup(pipe_c2p[PIPE_READ]);

            // close file descriptors for input/output
            close(pipe_c2p[PIPE_READ]);
            close(pipe_c2p[PIPE_WRITE]);
            execlp("wc","-l",NULL);
            
            // errors if not exited
            perror("execlp: wc -l");
            exit(1);
        }
    }
    printf("main    :end\n");
    return 0;
}