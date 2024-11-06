#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
    // shmget returns an identifier in shmid
    int shmid = shmget((key_t)65, 1024, 0666);

    // shmat to attach to shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0);

    // Read data from shared memory
    printf("Data read from memory: %s\n", str);

    // Detach from shared memory
    shmdt(str);

    // Optionally, destroy the shared memory (uncomment if needed)
    // shmctl(shmid, IPC_RMID, NULL);

    return 0;
}