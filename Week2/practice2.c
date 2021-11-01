#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define LOOP_ON 0
#define LOOP_OFF 1

#define SIG_OK 0

// Global variable
int loopFlag = LOOP_ON;

// Prototype
void sigHandler(int sigNo);

int main()
{
    printf("main    : start\n");
    // signal handler function registration
    
    signal(SIGINT,sigHandler);

    // Loop while on
    while(loopFlag == LOOP_ON)
    {
        printf("Looping\n");
        sleep(1);
    }

    printf("main:   end\n");
    return SIG_OK;
}

void sigHandler(int sigNo)
{
    printf("Signal No. %d\n",sigNo);
    loopFlag = LOOP_OFF;
    return;
}