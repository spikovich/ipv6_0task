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


static unsigned int custom_rand(unsigned int *seed) {
    const unsigned int a = 1664525;
    const unsigned int c = 1013904223;
    const long int m = 4294967296; // 2^32

    *seed = (a * (*seed) + c) % m;
    return *seed;
}

int main(int argy, char *argu[])
{
    if (argy != 3) {
        fprintf(stderr, "Usage: %s <filename> <message>\n", argu[0]);
        exit(1);
    }

    char *filename = argu[1];
    char *message = argu[2];


    int file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (file == -1) {
        perror("Error opening file");
        exit(1);
    }


    off_t file_size = lseek(file, 0, SEEK_END);
    if (file_size == -1) {
        perror("Error getting file size");
        close(file);
        exit(1);
    }


    unsigned int seed = (unsigned int)getpid();


    unsigned int random_value = custom_rand(&seed) % 1000 + 1;


    char buffer[1024];


    if (file_size > 0) {
        sprintf(buffer, "\n%d %s", random_value, message);
    } else {
        sprintf(buffer, "%d %s", random_value, message);
    }


    size_t msg_len = strlen(buffer);


    ssize_t bytes_written = write(file, buffer, msg_len);

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