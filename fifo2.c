#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define N 100

int main() {
    int fd1, sent, word;
    
    // FIFO file path
    char *myfifo = "/tmp/myfifo";
    
    // Creating the fifo
    mkfifo(myfifo, 0666);
    
    char str1[N], str2[N];
    while (1) {
        // First open in read mode
        fd1 = open(myfifo, O_RDONLY);
        read(fd1, str1, N);
        
        // Print the read string
        printf("User1: %s\n", str1);
        close(fd1);

        // Counting characters, words and sentences
        sent = 0;
        word = 0;
        
        int i;
        for(i = 0; str1[i] != '\0'; i++){
        	if(str1[i] == ' ')
        		word++;
        	if(str1[i] == '.')
        		sent++;
        }
        
        sprintf(str2,"Characters: %d \tWords : %d \tSentences : %d", i-1, word+1, sent);
		
        // Now open in write mode and write the count
        fd1 = open(myfifo, O_WRONLY);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    
    return 0;
}