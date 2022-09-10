#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#define N 9
#define BUFFER_SIZE 5
#define THREAD_NUM 10

int p_index = 0, c_index = 0, buffer[BUFFER_SIZE] = {0};
// fill to check if there is fill
int fill = 0, empty = BUFFER_SIZE;
pthread_mutex_t lock;
pthread_cond_t producerCond;
int take1 = 0;

void *producer()
{
    for (int i = 0; i < N; i++)
    {
        if (fill < BUFFER_SIZE)
        {
            pthread_mutex_lock(&lock);
            /*
            while(fill==BUFFER_SIZE){
                pthread_cond_signal(&producerCond);
            }
            */
            buffer[p_index] = rand() % 10 + 1;
            printf("produced %d at index %d\n", buffer[p_index], p_index);
            fill++;
            p_index = (p_index + 1) % BUFFER_SIZE;
            pthread_cond_signal(&producerCond);
            if (i == N - 1 && N % 2 == 1)
            {
                take1 = 1;
            }
            pthread_mutex_unlock(&lock);
            usleep(2);
        }
        else
        {
            while (fill == BUFFER_SIZE)
            {
                pthread_cond_signal(&producerCond);
            }
        }
    }
}

void *consumer()
{
    for (int i = 0; i < N / 2 + 1; i++)
    {
        pthread_mutex_lock(&lock);
        if (take1)
        {
            int x = buffer[c_index];
            fill--;
            printf("consumed %d at index %d\nconsumption ended\n", x, c_index);
            pthread_mutex_unlock(&lock);
            usleep(1);
            pthread_exit(0);
        }
        while (fill < 2)
        {
            printf("Not enough to consume...\n");
            // usleep(1);
            pthread_cond_wait(&producerCond, &lock);
            // lock mutex -> unlock
            //  signal wait
            //  mutex lock
        }
        fill -= 2;
        int x = buffer[c_index], y = buffer[(c_index + 1) % BUFFER_SIZE];
        buffer[c_index] = 0;
        buffer[(c_index + 1) % BUFFER_SIZE] = 0;
        printf("consumed %d and %d  at index %d and %d\n", x, y, c_index, (c_index + 1) % BUFFER_SIZE);
        c_index = (c_index + 2) % BUFFER_SIZE;
        pthread_mutex_unlock(&lock);
        usleep(1);
    }
}

int main()
{

    srand(time(NULL));
    pthread_t th[2];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&producerCond, NULL);

    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
            {
                perror("failed  to start thread\n");
                exit(1);
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0)
            {
                perror("failed  to start thread\n");
                exit(1);
            }
        }
    }

    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("failed to join thread\n");
            exit(1);
        }
    }

    pthread_cond_destroy(&producerCond);
    pthread_mutex_destroy(&lock);
    return 0;
}
