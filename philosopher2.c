#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define N 5     //number of philosophers
#define LEFT (ph_num+4) % N
#define RIGHT (ph_num+1) % N

enum {THINKING, HUNGRY, EATING} position;
sem_t mutex;
sem_t S[N];

void * philospher(void *num);
void pick_up(int);
void put_down(int);
void test(int);

int state[N];
int phil_num[N]={0,1,2,3,4};


void *philospher(void *num)
{
    while(1)
    {
        int *i = num;
        sleep(1);
        pick_up(*i);
        sleep(0);
        put_down(*i);
    }
}

void pick_up(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = HUNGRY;
    printf("Philosopher %d is Hungry\n",ph_num+1);
    test(ph_num);
    //UNLOCK SEMAPHORE.
    sem_post(&mutex);
    sem_wait(&S[ph_num]);
    sleep(1);
}

void test(int ph_num)
{
    if (state[LEFT] != EATING && state[ph_num] == HUNGRY &&  state[RIGHT] != EATING)
    {
    state[ph_num] = EATING;
    sleep(2);
    printf("Philosopher %d picking up chopstick %d and %d\n"
        ,ph_num+1,LEFT+1,ph_num+1);
    printf("Philosopher %d is Eating\n",ph_num+1);
    sem_post(&S[ph_num]);
    }
}

void put_down(int ph_num)
{
    sem_wait(&mutex);
    state[ph_num] = THINKING;

    printf("Philosopher %d is Thinking\n",ph_num+1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}


int main()
{
    int i;
    pthread_t thread_id[N];
    sem_init(&mutex,0,1);
    for(i=0;i<N;i++)
    {
        sem_init(&S[i],0,0);
    }
    for(i=0;i<N;i++)
    {
        pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
        printf("Philosopher %d is thinking\n",i+1);
    }
    for(i=0;i<N;i++)
    {
     pthread_join(thread_id[i],NULL);
    }
    return 1;
}

//Possible Output:
/*
Philosopher 1 is thinking
Philosopher 2 is thinking
Philosopher 3 is thinking
Philosopher 4 is thinking
Philosopher 5 is thinking
Philosopher 5 is Hungry
Philosopher 5 picking up chopstick 4 and 5
Philosopher 5 is Eating
Philosopher 4 is Hungry
Philosopher 3 is Hungry
Philosopher 3 picking up chopstick 2 and 3
Philosopher 3 is Eating
Philosopher 2 is Hungry
Philosopher 1 is Hungry
Philosopher 5 is Thinking
Philosopher 1 picking up chopstick 5 and 1
Philosopher 1 is Eating
Philosopher 3 is Thinking
Philosopher 4 picking up chopstick 3 and 4
Philosopher 4 is Eating
Philosopher 1 is Thinking
Philosopher 2 picking up chopstick 1 and 2
Philosopher 2 is Eating
Philosopher 5 is Hungry
Philosopher 3 is Hungry
Philosopher 4 is Thinking
Philosopher 5 picking up chopstick 4 and 5
Philosopher 5 is Eating
Philosopher 2 is Thinking
Philosopher 3 picking up chopstick 2 and 3
Philosopher 3 is Eating
Philosopher 1 is Hungry
Philosopher 4 is Hungry
Philosopher 5 is Thinking
*/
