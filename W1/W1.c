//
// Created by DDSPICH on 2/18/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argy, char *argu[])
{

    if (argy != 3) {
        fprintf(stderr, "Usage: %s <filename> <message>\n", argu[0]);
        exit(1);
    }


    char *filename = argu[1];
    char *message = argu[2];


    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (file == -1) {
        perror("Error opening file");
        exit(1);
    }


    size_t msg_len = strlen(message);


    ssize_t bytes_written = write(file, message, msg_len);

    if (bytes_written == -1) {
        perror("Error writing to file");
        close(file);
        exit(1);
    }


    if (bytes_written != (ssize_t)msg_len) {
        fprintf(stderr, "Partial write: %zd of %zu bytes written\n",
                bytes_written, msg_len);
        close(file);
        exit(1);
    }


    if (close(file) == -1) {
        perror("Error closing file");
        exit(1);
    }

    return 0;
}