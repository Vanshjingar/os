#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>  // Include for wait()

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

void process1();
void process2();

int main() {
    // Create two named pipes (FIFOs)
    if (mkfifo(FIFO1, 0666) < 0) {
        perror("Error creating FIFO1");
        exit(1);
    }
    if (mkfifo(FIFO2, 0666) < 0) {
        perror("Error creating FIFO2");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process -> Process 2
        process2();
    } else {
        // Parent process -> Process 1
        process1();

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}

void process1() {
    char input[1024], output[1024];
    int fd1, fd2;

    // Take input from the user
    printf("Enter text (end with newline):\n");
    fgets(input, sizeof(input), stdin);

    // Write user input to FIFO1
    fd1 = open(FIFO1, O_WRONLY);
    if (fd1 < 0) {
        perror("Error opening FIFO1 for writing");
        exit(1);
    }
    write(fd1, input, strlen(input) + 1); // Including null-terminator
    close(fd1);

    // Wait and read from FIFO2
    fd2 = open(FIFO2, O_RDONLY);
    if (fd2 < 0) {
        perror("Error opening FIFO2 for reading");
        exit(1);
    }
    read(fd2, output, sizeof(output));
    close(fd2);

    printf("\n===== Output from Process 2 =====\n%s\n", output);
}

void process2() {
    char buffer[1024];
    int fd1, fd2;

    // Read from FIFO1
    fd1 = open(FIFO1, O_RDONLY);
    if (fd1 < 0) {
        perror("Error opening FIFO1 for reading");
        exit(1);
    }
    read(fd1, buffer, sizeof(buffer));
    close(fd1);

    int chars = 0, words = 0, lines = 0;
    int inWord = 0;

    // Analyze the content
    for (int i = 0; buffer[i] != '\0'; i++) {
        chars++;
        if (buffer[i] == '\n') lines++;
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t') inWord = 0;
        else if (!inWord) { inWord = 1; words++; }
    }

    // Write the analysis to a file
    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fprintf(fp, "Input: %s\n", buffer);
    fprintf(fp, "Number of characters: %d\n", chars);
    fprintf(fp, "Number of words: %d\n", words);
    fprintf(fp, "Number of lines: %d\n", lines);
    fclose(fp);

    // Read the contents of the file and send them to FIFO2
    fp = fopen("output.txt", "r");
    if (fp == NULL) {
        perror("Error reading file");
        exit(1);
    }

    char fileData[1024];
    size_t bytesRead = fread(fileData, sizeof(char), sizeof(fileData), fp);
    fileData[bytesRead] = '\0'; // Ensure null termination
    fclose(fp);

    // Write to FIFO2
    fd2 = open(FIFO2, O_WRONLY);
    if (fd2 < 0) {
        perror("Error opening FIFO2 for writing");
        exit(1);
    }
    write(fd2, fileData, bytesRead + 1); // Write null-terminated string
    close(fd2);
}

