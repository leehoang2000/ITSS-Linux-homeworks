#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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
    int bufSize;
    int readSize = 0;
    int childStatus = 0;
    int stdoutBackup = 0;

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

            // Reading process is suspended until data exists in write head
            read(pipe_c2p[PIPE_READ], buf, bufSize);

            // Redirect stdout to pipe write head
            stdoutBackup = dup(STD_OUT);
            close(STD_OUT);
            dup(pipe_c2p[PIPE_WRITE]);

            // Execute ls with options from parent
            execlp("ls",buf,NULL);
            fprintf(stderr, "execlp: ls %s",buf);
            exit(1);
        }
        default: // parent process
        {
            printf("parentProcess start\n");

            // Write to pipe
            strcpy(buf, "-ltr");
            bufSize = strlen(buf)+1;
            write(pipe_c2p[PIPE_WRITE], buf, bufSize);

            wait(&childStatus);

            // Redirect stdin to pipe read head
            close(STD_IN);
            dup(pipe_c2p[PIPE_READ]);

            // Restore stdout
            close(pipe_c2p[PIPE_WRITE]);
            dup(stdoutBackup);

            // Execute wc -l on stdin
            execlp("wc","wc","-l",NULL);
            perror("execlp: wc -l");
            exit(1);
        }
    }
}