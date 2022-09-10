#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define N 41
#define NBUF 30

#define MIN 100000
#define MAX 600000

#define THREAD_SHARE 0
#define PROCESS_SHARE 1

sem_t *empty, *fill, *lock;
int shm1, shm2, shm3, shm4, shm5;

int pindex = 0, cindex = 0;
int counter = 1;
int *buffer;

int *take1;

void *producer()
{
    for (int i = 0; i < N; i++)
    {
        sem_wait(empty);
        sem_wait(lock);

        buffer[pindex] = counter;
        counter++;
        printf("produced at index %d item : %d\n", pindex, buffer[pindex]);
        pindex = (pindex + 1) % NBUF;

        if (i == N - 1 && N % 2 == 1)
            *take1 = 1;

        sem_post(lock);
        sem_post(fill);

        // unsigned int rval = MIN + (unsigned int)(rand() * 1.0 * (MAX - MIN)) / RAND_MAX;
        // usleep(rval);
    }
}

void *consumer()
{
    for (int i = 0; i < N / 2; i++)
    {
        sem_wait(fill);
        if (*take1 == 0)
        {
            // printf("HELLO SIMPLE - GANGWAR\n") ;
            sem_wait(fill);
        }
        sem_wait(lock);

        if (*take1 == 1)
        {
            int item = buffer[cindex];
            printf("consumed at index %d item: %d \n", cindex, item);
            break;
        }
        else
        {

            int item = buffer[cindex];
            printf("consumed at index %d item: %d \n", cindex, item);
            cindex = (cindex + 1) % NBUF;
            int item1 = buffer[cindex];
            printf("consumed at index %d item: %d \n", cindex, item1);
            cindex = (cindex + 1) % N;
        }

        sem_post(lock);
        sem_post(empty);
        if (*take1 == 0)
            sem_post(empty);

        // unsigned int rval = MIN + (unsigned int)(rand() * 1.0 * (MAX - MIN)) / RAND_MAX;
        // usleep(rval);
    }
}

int main()
{
    int t1_id, t2_id;
    int rc;

    shm1 = shmget(1234, sizeof(sem_t), IPC_CREAT | 0777);
    if (shm1 < 0)
    {
        perror("PARENT: Failed to get shared memory segment in shm1");
        exit(0);
    }
    empty = (sem_t *)shmat(shm1, (void *)0, 0);
    if ((void *)-1 == empty)
    {
        perror("Failed: \n");
        exit(0);
    }

    shm2 = shmget(2345, sizeof(sem_t), IPC_CREAT | 0777);
    if (shm2 < 0)
    {
        perror("PARENT: Failed to get shared memory segment in shm2");
        exit(0);
    }
    fill = (sem_t *)shmat(shm2, (void *)0, 0);
    if ((void *)-1 == fill)
    {
        perror("Failed: \n");
        exit(0);
    }

    shm3 = shmget(3456, NBUF * sizeof(int), IPC_CREAT | 0777);
    if (shm3 < 0)
    {
        perror("PARENT: Failed to get shared memory segment in shm3");
        exit(0);
    }
    buffer = (int *)shmat(shm3, (void *)0, 0);
    if (NULL == buffer)
    {
        perror("Failed: \n");
        exit(0);
    }

    shm4 = shmget(4567, sizeof(int), IPC_CREAT | 0777);
    if (shm4 < 0)
    {

        perror("PARENT: Failed to get shared memory segment in shm4");
        exit(0);
    }
    take1 = (int *)shmat(shm4, (void *)0, 0);
    if ((void *)-1 == take1)
    {
        perror("Failed: \n");
        exit(0);
    }

    shm5 = shmget(6789, sizeof(sem_t), IPC_CREAT | 0777);
    if (shm5 < 0)
    {
        perror("PARENT: Failed to get shared memory segment in shm5\n");
        exit(0);
    }
    lock = (sem_t *)shmat(shm5, (void *)0, 0);
    if ((void *)-1 == lock)
    {
        perror("Failed: \n");
        exit(0);
    }

    sem_init(empty, PROCESS_SHARE, N);
    sem_init(fill, PROCESS_SHARE, 0);
    sem_init(lock, PROCESS_SHARE, 1);
    *take1 = 0;

    rc = fork();
    if (-1 == rc)
    {
        printf("Failed to fork\n");
        exit(0);
    }
    if (0 == rc)
    {
        printf("CHILD:   %d     parent: %d \n", getpid(), getppid());
        srand(200);
        producer();
    }
    else
    {
        printf("PARENT:   %d     parent: %d \n", getpid(), getppid());
        srand(100);
        consumer();
    }

    sem_destroy(fill);
    sem_destroy(empty);
    sem_destroy(lock);

    shmdt(fill);
    shmdt(empty);
    shmdt(buffer);
    shmdt(lock);
    shmdt(take1);

    shmctl(shm1, IPC_RMID, NULL);
    shmctl(shm2, IPC_RMID, NULL);
    shmctl(shm3, IPC_RMID, NULL);
    shmctl(shm5, IPC_RMID, NULL);
    shmctl(shm4, IPC_RMID, NULL);

    return 0;
}
