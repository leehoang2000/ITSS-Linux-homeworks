#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define SIG_OK 0
#define SIG_NG -1
#define LOOP_ON 0
#define LOOP_OFF 1

#define SIGHUP 1

// Global variable
int loopFlag = LOOP_ON;

// Prototype
void sigHandleSighup(int sigNo);

int main()
{
    printf("main    : start\n");
    // signal handler function registration
    signal(SIGHUP,sigHandleSighup);

    // Loop while on
    while(loopFlag == LOOP_ON)
    {
        printf("Looping\n");
    }

    printf("main:   end\n");
    return SIG_OK;
}

void sigHandleSighup(int sigNo)
{
    printf("Signal No. %d\n",sigNo);
    loopFlag = LOOP_OFF;
    return;
}