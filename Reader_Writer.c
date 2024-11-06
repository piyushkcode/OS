#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define max 10

int readCount = 0, var = 0;
sem_t writer, reader;
pthread_mutex_t mutex;

void *readerFunc(void *args) {
    while (1) {
        sem_wait(&reader); // Control access for readers (if needed to limit simultaneous readers)
        pthread_mutex_lock(&mutex);
        readCount++;
        if (readCount == 1) {
            sem_wait(&writer); // First reader locks writer
        }
        pthread_mutex_unlock(&mutex);

        // Read operation
        printf("\nReading is performed and the value read is: %d", var);
        sleep(1); // Simulate reading time

        pthread_mutex_lock(&mutex);
        readCount--;
        if (readCount == 0) {
            sem_post(&writer); // Last reader unlocks writer
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&reader); // Allow next reader
    }
}

void *writerFunc(void *args) {
    while (1) {
        sem_wait(&writer); // Lock for writing
        var++;
        printf("\n\nWriting is performed and the value changed to: %d", var);
        sem_post(&writer); // Unlock for reading
        sleep(1); // Simulate writing time
    }
}

int main() {
    int read, write, i;

    if (sem_init(&writer, 0, 1) != 0 || sem_init(&reader, 0, max) != 0) {
        printf("\nSEMAPHORE INITIALIZATION FAILED!\n");
        exit(1);
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("\nMUTEX INITIALIZATION FAILED!\n");
        exit(1);
    }

    printf("\nEnter the number of readers: ");
    scanf("%d", &read);
    printf("\nEnter the number of writers: ");
    scanf("%d", &write);

    if (read > max || write > max) {
        printf("Error: Number of readers or writers exceeds maximum limit of %d\n", max);
        exit(1);
    }

    pthread_t reads[max], writes[max];

    // Creating writer threads
    for (i = 0; i < write; i++) {
        if (pthread_create(&writes[i], NULL, writerFunc, NULL) != 0) {
            printf("\nWriter thread no %d creation failed!", i + 1);
        } else {
            printf("\n\nWriter thread no %d created!", i + 1);
        }
    }

    // Creating reader threads
    for (i = 0; i < read; i++) {
        if (pthread_create(&reads[i], NULL, readerFunc, NULL) != 0) {
            printf("\nReader thread no %d creation failed!", i + 1);
        } else {
            printf("\n\nReader thread no %d created!", i + 1);
        }
    }

    // Joining all threads
    for (i = 0; i < read; i++) {
        pthread_join(reads[i], NULL);
    }
    for (i = 0; i < write; i++) {
        pthread_join(writes[i], NULL);
    }

    // Destroy the semaphore and mutex
    sem_destroy(&writer);
    sem_destroy(&reader);
    pthread_mutex_destroy(&mutex);

    return 0;
}