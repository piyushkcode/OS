#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define N 100

int main() {
    int fd;

    // FIFO file path
    char *myfifo = "/tmp/myfifo";

    // Creating the fifo
    mkfifo(myfifo, 0666);

    char arr1[N], arr2[N];
    while (1) {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        // Take an input arr2 from user
        printf("\nEnter String : ");
        fgets(arr2, N, stdin);

        // Write the input arr2 on FIFO
        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        // Open FIFO for Read only N
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, arr1, sizeof(arr1));

        // Print the read message
        printf("User2: %s\n", arr1);
        close(fd);
    }
    return 0;
}