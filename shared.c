#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
    // shmget returns an identifier in shmid
    int shmid = shmget((key_t)65, 1024, 0666 | IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char *)shmat(shmid, (void *)0, 0);

    // Write data to the shared memory
    printf("Write Data: ");
    fgets(str, 1024, stdin);  // safer alternative to gets()

    printf("Data written in memory: %s\n", str);

    // Detach from shared memory
    shmdt(str);

    // Keep server running to allow client to read from shared memory
    while (1);

    return 0;
}