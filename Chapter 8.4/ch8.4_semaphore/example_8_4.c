#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEYFILE_PATH "keyfile"
#define ID 'S'
#define SEM_OK 0
#define SEM_NG -1

#define UNUSED_ARG 0

int main()
{
    int semid, semval;
    key_t keyval;
    struct sembuf buff;

    printf("processA:   start\n");
    // Generates an IPC key from an existing file
    // and ID
    keyval = ftok(KEYFILE_PATH, (int)ID);

    // Create semaphore set of 1 semaphore, set is associated with the IPC key
    semid = semget(keyval, 1, IPC_CREAT | 0660);
    printf("processA(sem): semID = %d\n",semid);

    // Sembuf structure setting for semaphore operation
    buff.sem_num = (ushort)0; // Number of semaphore to operate on
      
    // Wait: Try to decrement semaphore (Linux should've already initialized it to 0, so this process should be blocked)
    buff.sem_op = (short)-4;

    // Flag: Normal flag: semncnt (# of threads waiting) increments by 1
    buff.sem_flg = (short)0;

    // Semaphore operation on 1 semaphore in set
    semop(semid,&buff,1);
    printf("processA(sem): sem_op = %d\n",buff.sem_op);

    /* Below should only execute once semaphore is signaled */
    
    // Semaphore control: Get semval of semaphore with ID 0
    semval = semctl(semid, 0, GETVAL, UNUSED_ARG);
    printf("processA(sem): semval = %d\n",semval);

    if(semval == 0)
    {
        // Immediately remove semaphore set
        if( semctl(semid, UNUSED_ARG, IPC_RMID, UNUSED_ARG) == SEM_NG )
        {
            perror("processA    : semctl");
            exit(1);
        }
    }
    printf("processA    : end\n");
    return 0;
}