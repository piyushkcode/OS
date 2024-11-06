#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void) {
    char filename[BUFFER_SIZE] = "thefile.txt";
    char read_msg[BUFFER_SIZE];
    char ch;
    int fd[2], fd1[2];
    FILE *filePointer; 
    pid_t pid;

    // Create two pipes
    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if (pipe(fd1) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();
    if (pid < 0) { 
        // Error occurred
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    if (pid > 0) { 
        // Parent process
        close(fd[READ_END]);  // Close unused read end
        // Write the filename to the pipe
        write(fd[WRITE_END], filename, strlen(filename) + 1);
        close(fd[WRITE_END]);  // Close the write end

        // Wait for child to write to file
        wait(NULL); 

        // Parent reads the filename back from the pipe and then reads the file contents
        close(fd1[WRITE_END]);  // Close unused write end
        read(fd1[READ_END], read_msg, BUFFER_SIZE);
        printf("\nParent read the filename as %s", read_msg);
        close(fd1[READ_END]);  // Close the read end

        // Open the file and display contents
        filePointer = fopen(read_msg, "r");
        if (filePointer == NULL) {
            fprintf(stderr, "Failed to open file\n");
            return 1;
        }

        ch = fgetc(filePointer);
        while (ch != EOF) {
            printf("%c", ch);
            ch = fgetc(filePointer);
        }

        fclose(filePointer);
    } else { 
        // Child process
        close(fd[WRITE_END]);  // Close unused write end
        // Read filename from parent
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("\nChild read the filename as %s", read_msg);
        close(fd[READ_END]);  // Close the read end

        // Write some content to the file
        filePointer = fopen(read_msg, "w");
        if (filePointer == NULL) {
            fprintf(stderr, "Failed to open file for writing\n");
            return 1;
        }
        fprintf(filePointer, "%s", "This is some random data written by child");
        fclose(filePointer);

        // Send filename back to parent
        close(fd1[READ_END]);  // Close unused read end
        write(fd1[WRITE_END], read_msg, strlen(read_msg) + 1);
        close(fd1[WRITE_END]);  // Close the write end
    }

    return 0;
}