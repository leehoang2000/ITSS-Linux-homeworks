#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHIMSIZE 10
#define KEYFILE_PATH "keyfilepath"
#define PROJ_CHAR 'A'

int main()
{
    int i;
    int shmsize;
    int shmid;
    key_t keyval;
    int *ptr;
    int *head;

    keyval = ftok(KEYFILE_PATH, (int)PROJ_CHAR);
    shmsize = SHIMSIZE;

    //Get the shared memory ID
    if ((shmid = shmget(keyval, shmsize * sizeof(int),
                        IPC_CREAT | 0660)) == -1)
    {
        perror("shmget");
        exit(1);
    }
    //Attach the shared memory
    if (((head = ptr = (int *)shmat(shmid, 0, 0)) == (int *)-1))
    {
        perror("shmat");
        exit(1);
    }

    //Print
    for (i = 0; i != shmsize; i++)
    {
        printf("%c",*ptr++);
    }

    //Detach the shared memory
    if (shmdt((void *)head) == -1)
    {
        perror("shmdt");
        exit(1);
    }
    //Delete the shared memory
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        perror("shmctl");
        exit(1);
    }

    return 0;
}