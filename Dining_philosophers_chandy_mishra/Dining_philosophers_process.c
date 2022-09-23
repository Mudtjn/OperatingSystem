#include <stdio.h>
#include <stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h> 
#include <pthread.h>
#include <semaphore.h> 
#define N 5
#define ITER 10
#define key 1234

typedef struct {
    int with;
    int status;
    // status : 1-> dirty,  0-> clean 
} chopstick;

typedef struct { 
    int status;
    // status : 0-> hungry , 1-> eating, 2-> thinking
} philosopher;

typedef struct shared{
    philosopher philosopher_array[N];
    chopstick chopstick_array[N] ; 
    int count;
}shared;


int shmid;

void func ( int index ) {

    // interact with index philosopher ;
    shared* shmp = (shared *)shmat(shmid, NULL,0);
    while(1){
        shmp->philosopher_array[index].status = 0; // put hungry

        // wait till philosopher is hungry and both forks dirty
        // if(index == 0){
        //     printf("||||||||||||||||||||||||||||||||||||||||||\n");
        //     exit(EXIT_FAILURE);
        // }

        while(shmp->chopstick_array[index].with == index && shmp->chopstick_array[index].status == 1){
            printf("Philosopher %d is hungry----------------\n", index);
            sleep(1) ;
        }



        while(shmp->chopstick_array[(index-1 + N)%N].with == index && shmp->chopstick_array[(index-1 + N)%N].status==1){
            printf("Philosopher %d is hungry++++++++++++++++\n",index); 
            sleep(1) ;
        }


        // check -> neighbours free
        int p1 = (index + N - 1) % N, p2 = (index + 1) % N;
        if (shmp->chopstick_array[index].with != index){
            // request
            while ( shmp->philosopher_array[p2].status == 1 ){} 
                // change state
            shmp->chopstick_array[index].status = 0;
            shmp->chopstick_array[index].with = index; 
        }
        if (shmp->chopstick_array[p1].with != index){
            // request
            while (shmp->philosopher_array[p1].status == 1){}
            shmp->chopstick_array[p1].status = 0;
            shmp->chopstick_array[p1].with = index;
            
        }
        // have two forks
        printf("Chopsticks %d and %d are FREE.....\n", index, p1);

        // eat()
        shmp->philosopher_array[index].status = 1;
        printf("Philoso[pher %d is eating \n", index);
        // exit(EXIT_FAILURE);
        // fork assign dirty
        shmp->chopstick_array[index].status = 1; // chopstic status = 1 means dirty
        shmp->chopstick_array[p1].status = 1;
        printf("chopsticks %d and %d are dirty..\n", index, p1) ;

        // thinking
        shmp->philosopher_array[index].status = 2; // philosopher status = 0 means hungry and 2 means thinking
        printf("Philosopher %d is thinking...\n", index);

    }
    shmp->count++;
        shmdt(shmp);
    // sleep(1);
    // free(index);
}

int main(){

    shmid = shmget(key, sizeof(shared), 0777|IPC_CREAT);

    shared* shmp = (shared *)shmat(shmid, NULL,0);
    pid_t parent = getpid();

    for (int i = 0; i < N; i++){
        shmp->chopstick_array[i].status = 1;
        shmp->philosopher_array[i].status = 0;
    }

    for (int i = 0; i < N - 1; i++){
        shmp->chopstick_array[i].with = (i+1)%N;  
    }
    shmp->chopstick_array[N-1].with = N-1; 

    shmp->count = 0;
    
    int i = 0;
    for(i=0; i<N; i++){
        int rc = fork();
        if(rc == -1){
            shmdt(shmp);
            shmctl(shmid, IPC_RMID, 0);
            printf("fork cause error");
            exit(EXIT_FAILURE);
        }

        else if(rc == 0){
            break;
        }
    }

    if(getpid() == parent){
        while(shmp->count != N);

        shmdt(shmp);
        shmctl(shmid, IPC_RMID, 0);
        printf("completed >>>>>>>> \n");
    }
    
    else{
        func(i);
        shmdt(shmp);
    }
    return 0;
}

