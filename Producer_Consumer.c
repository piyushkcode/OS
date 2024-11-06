#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define buff_size 5  // Adjusting buffer size to match the defined buffer

void init();
void *producer(void *num);
void *consumer(void *num);

typedef struct {
    int in, out, buffer[buff_size];
    sem_t full, empty;
    pthread_mutex_t lock;
} BUFFER;

BUFFER b1;
int item = 1;

void init() {
    b1.in = 0;
    b1.out = 0;
    sem_init(&b1.empty, 0, buff_size); // Semaphore initialized with buffer size
    sem_init(&b1.full, 0, 0);          // Semaphore initialized to 0

    pthread_mutex_init(&b1.lock, NULL);
}

void *producer(void *num) {
    int i = *(int *)num;
    printf("\n-------- Thread for Producer %d started the process for production of Items.\n", i);
    while (1) {
        int sleep_time = (rand() % 10) + 1;
        sleep(sleep_time);
        
        sem_wait(&b1.empty); // Wait if buffer is full
        pthread_mutex_lock(&b1.lock);
        
        printf("\nProducer %d produced item %d (|)", i, item);
        b1.buffer[b1.in % buff_size] = item++;
        b1.in = (b1.in + 1) % buff_size;
        
        pthread_mutex_unlock(&b1.lock);
        sem_post(&b1.full); // Signal that there is an item to consume
    }
}

void *consumer(void *num) {
    int i = *(int *)num;
    printf("\n-------- Thread for Consumer %d started the process for consumption of Items.\n", i);
    while (1) {
        int sleep_time = (rand() % 10) + 1;
        sleep(sleep_time);
        
        sem_wait(&b1.full); // Wait if buffer is empty
        pthread_mutex_lock(&b1.lock);
        
        int cons = b1.buffer[b1.out % buff_size];
        printf("\n-------------------------------------------- Consumer %d consumed item %d (x)", i, cons);
        b1.out = (b1.out + 1) % buff_size;
        
        pthread_mutex_unlock(&b1.lock);
        sem_post(&b1.empty); // Signal that there is an empty space
    }
}

int main() {
    int i, np, nc;
    pthread_t pid[buff_size], cid[buff_size];
    int c[] = {1, 2, 3, 4, 5}; // Thread identifiers

    printf("\nEnter the number of Producers: ");
    scanf("%d", &np);

    printf("\nEnter the number of Consumers: ");
    scanf("%d", &nc);

    init();

    for (i = 0; i < np; i++) {
        pthread_create(&pid[i], NULL, producer, &c[i]);
        printf("\nThread for Producer %d Created.\n", i);
    }
    for (i = 0; i < nc; i++) {
        pthread_create(&cid[i], NULL, consumer, &c[i]);
        printf("\nThread for Consumer %d Created.\n", i);
    }

    for (i = 0; i < np; i++) {
        pthread_join(pid[i], NULL);
        printf("\n-------- Thread for Producer %d Joined at End.\n", i);
    }
    for (i = 0; i < nc; i++) {
        pthread_join(cid[i], NULL);
        printf("\n-------- Thread for Consumer %d Joined at End.\n", i);
    }

    return 0;
}