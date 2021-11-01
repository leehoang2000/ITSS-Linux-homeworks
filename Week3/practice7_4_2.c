#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROC_OK 0
#define PROC_NG -1

int loop_on = 1;

void sigHandler(int sigNo)
{
    printf("Signal No. %d\n",sigNo);
    loop_on = 0;
}

int main()
{
    printf("main    : start\n");

    char* args[] = {/*ls,*/"-l",NULL};
    int iRet = 0;
    pid_t pid = 0;
    int status = 0;

    switch(pid = fork())
    {
        case -1:
        {
            perror("process error: Generate fork");
            exit(1);
        }
        case 0:
        {
            // Child process
// Exercise 2: Child does not exec, kill child from console

            printf("execv ret = %d\n",iRet);
            if(iRet == PROC_NG)
            {
                exit(1);
            }    
            // Register signal handler
            signal(SIGABRT,sigHandler);
            while(loop_on)
            {
                sleep(1);
            }
            exit(SIGABRT);
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