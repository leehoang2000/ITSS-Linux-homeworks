#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROC_OK 0
#define PROC_NG -1

int main()
{
    printf("main    : start\n");

    char* args[] = {/*ls,*/"-l",NULL};
    int iRet = 0;
    pid_t pid = 0;
    int status = 0;

// Exercise 4: Generate and end multiple child processes
    for(int i=0;i<2;i++)
    {
        pid = fork();
    }
    switch(pid)
    {
        case -1:
        {
            perror("process error: Generate fork");
            exit(1);
        }
        case 0:
        {
            // Child process
            iRet = execv("/bin/ls",args);

            printf("execv ret = %d\n",iRet);
            if(iRet == PROC_NG)
            {
                exit(1);
            }    
        }
        default:
        {
            // Parent process
            printf("Parent Process ChildProcId = %d\n",pid);
            pid = wait(&status);
            printf("child pid=%d,child status=%d\n",pid,status);
            break;
        }
    }

    printf("main    : end\n");
    return 0;
}