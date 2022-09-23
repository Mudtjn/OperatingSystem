#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 
#define N 5
#define ITER 10

typedef struct {
    int with;
    int status;
    // status : 1-> dirty,  0-> clean 
} chopstick;

typedef struct { 
    int status;
    // status : 0-> hungry , 1-> eating, 2-> thinking
} philosopher;

philosopher philosopher_array[N];
chopstick chopstick_array[N] ; 



void *func ( void*a ) {

    int index = *(int*)a;
    // interact with index philosopher ;
    while(1){
        philosopher_array[index].status = 0; // put hungry

        

        while(chopstick_array[index].with == index && chopstick_array[index].status == 1){
            printf("Philosopher %d is hungry----------------\n", index);
            sleep(1) ;
        }



        while(chopstick_array[(index-1 + N)%N].with == index && chopstick_array[(index-1 + N)%N].status==1){
            printf("Philosopher %d is hungry++++++++++++++++\n",index); 
            sleep(1) ;
        }


        // check -> neighbours free
        int p1 = (index + N - 1) % N, p2 = (index + 1) % N;
        if (chopstick_array[index].with != index){
            // request
            while ( philosopher_array[p2].status == 1 ){} 
                // change state
            chopstick_array[index].status = 0;
            chopstick_array[index].with = index; 
        }
        if (chopstick_array[p1].with != index){
            // request
            while (philosopher_array[p1].status == 1){}
            chopstick_array[p1].status = 0;
            chopstick_array[p1].with = index;
            
        }
        // have two forks
        printf("Chopsticks %d and %d are FREE.....\n", index, p1);

        // eat()
        philosopher_array[index].status = 1;
        printf("Philoso[pher %d is eating \n", index);
        // exit(EXIT_FAILURE);
        // fork assign dirty
        chopstick_array[index].status = 1; // chopstic status = 1 means dirty
        chopstick_array[p1].status = 1;
        printf("chopsticks %d and %d are dirty..\n", index, p1) ;

        // thinking
        philosopher_array[index].status = 2; // philosopher status = 0 means hungry and 2 means thinking
        printf("Philosopher %d is thinking...\n", index);
    }
    // sleep(1);
    // free(index);
}

int main(){

    for (int i = 0; i < N; i++){
        chopstick_array[i].status = 1;
        philosopher_array[i].status = 0;
    }

    for (int i = 0; i < N - 1; i++){
        chopstick_array[i].with = (i+1)%N;  
    }
    chopstick_array[N-1].with = N-1; 
    int ID[N]; 
    pthread_t th[N];
    for (int i = 0; i < N; i++){
        ID[i] = i ;
        // int *a = &x;
        if (pthread_create(&th[i], NULL, *func, (void *)&(ID[i])) != 0)
        {
            printf("Could not create thread");
            exit(EXIT_FAILURE);
        }

    }

    for (int i = 0; i < N; i++){
        pthread_join(th[i], NULL);
    }
    printf("completed >>>>>>>> \n");
    return 0;
}

