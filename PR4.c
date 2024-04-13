//Consumer Producer

// Producer - Consumer Problem
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int counter = 0;
sem_t empty, full, mutex;

void *producer(void *args)
{
    sem_wait(&empty); // wait
    sem_wait(&mutex);
    printf("Producer process produced %d\n", ++counter);
    sem_post(&mutex); // signal
    sem_post(&full);
}

void *consumer(void *args)
{
    sem_wait(&full);
    sem_wait(&mutex);
    printf("Consumer consumed the porcess %d\n", counter--);
    sem_post(&mutex);
    sem_post(&empty);
}

int main()
{
    sem_init(&empty, 1, 5);
    sem_init(&full, 1, 0);
    sem_init(&mutex, 1, 1);
    int m, n;
    printf("Enter the value of producer thread : ");
    scanf("%d", &m); // m producer
    printf("Enter the value of consumer thread : ");
    scanf("%d", &n); // n consumer

    pthread_t ptid[m];
    for (int i = 0; i < m; i++)
    {
        pthread_create(&ptid[i], NULL, producer, NULL);
    }
    for (int i = 0; i < m; i++)
    {
        pthread_join(ptid[i], NULL);
    }

    pthread_t tid[n];
    for (int i = 0; i < n; i++)
    {
        pthread_create(&tid[i], NULL, consumer, NULL);
        // sleep(1);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}














//read Writer 
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int counter = 0;
sem_t rw_mutex, wr_mutex;

void *reader(void *args)
{
    sem_wait(&rw_mutex);
    printf("Reader process is reading %d\n", ++counter);
    sem_post(&rw_mutex);
}

void *writer(void *args)
{
    sem_wait(&wr_mutex);
    printf("Writer process is modifying %d\n", counter--);
    sem_post(&wr_mutex);
}

int main()
{
    sem_init(&rw_mutex, 0, 1); // Initialize read-write mutex
    sem_init(&wr_mutex, 0, 1); // Initialize writer mutex

    int m, n;
    printf("Enter the value of reader threads: ");
    scanf("%d", &m); // Number of reader threads
    printf("Enter the value of writer threads: ");
    scanf("%d", &n); // Number of writer threads

    pthread_t ptid[m];
    for (int i = 0; i < m; i++)
    {
        pthread_create(&ptid[i], NULL, reader, NULL);
    }
    for (int i = 0; i < m; i++)
    {
        pthread_join(ptid[i], NULL);
    }

    pthread_t tid[n];
    for (int i = 0; i < n; i++)
    {
        pthread_create(&tid[i], NULL, writer, NULL);
    }
    for (int i = 0; i < n; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}













// Dining philosper
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void test(int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        // state that eating
        state[phnum] = EATING;

        sleep(2);

        printf("Philosopher %d takes fork %d and %d\n",
               phnum + 1, LEFT + 1, phnum + 1);

        printf("Philosopher %d is Eating\n", phnum + 1);

        // sem_post(&S[phnum]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}

// take up chopsticks
void take_fork(int phnum)
{

    sem_wait(&mutex);

    // state that hungry
    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    // eat if neighbours are not eating
    test(phnum);

    sem_post(&mutex);

    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);

    sleep(1);
}

// put down chopsticks
void put_fork(int phnum)
{

    sem_wait(&mutex);

    // state that thinking
    state[phnum] = THINKING;

    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);
}

void *philosopher(void *num)
{

    while (1)
    {

        int *i = num;

        sleep(1);

        take_fork(*i);

        sleep(0);

        put_fork(*i);
    }
}

int main()
{

    int i;
    pthread_t thread_id[N];

    // initialize the semaphores
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)

        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++)
    {

        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philosopher, &phil[i]);

        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)

        pthread_join(thread_id[i], NULL);
}
