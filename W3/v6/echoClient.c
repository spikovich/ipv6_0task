/* ========================================================================== */
/*                                                                            */
/*   echoClient.c                                                             */
/*   (c) 2012 Tomas Palenik                                                   */
/*                                                                            */
/*   Client opens a LOCAL-type socket and sends a message to server.          */
/*              The message should be a lo-case string.                       */
/*              After sending the message,                                    */
/*              client expects the server to reply with a response message.   */
/* ========================================================================== */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "settings.h"

char Buff[MAX_MSG_SIZE];  // Output message buffer
char Fubb[MAX_MSG_SIZE];  // Input message buffer
int Sock_fd = -1;

void cleanup(void) {
    if (Sock_fd != -1) {
        close(Sock_fd);
        Sock_fd = -1;
    }
}

int main(int argc, const char *argv[]) {
    int len;
    struct sockaddr_in6 address;
    int result;


    if (argc < 3) {
        printf("Usage: %s <server_ip> <message>\n", argv[0]);
        printf("client error: both server IP and message must be specified.\n");
        return 1;
    }

    atexit(cleanup);


    Sock_fd = socket(AF_INET6, SOCK_STREAM, 0);
    _runSafe(Sock_fd);


    memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_port = htons(5000);
    inet_pton(AF_INET6, argv[1], &address.sin6_addr);


    result = connect(Sock_fd, (struct sockaddr *)&address, sizeof(address));
    _runSafe(result);


    strncpy(Buff, argv[2], MAX_MSG_SIZE - 1);
    Buff[MAX_MSG_SIZE - 1] = '\0';
    printf("client sending message: %s\n", Buff);
    len = write(Sock_fd, Buff, strlen(Buff) + 1);
    _runSafe(len);


    len = read(Sock_fd, Fubb, MAX_MSG_SIZE);
    _runSafe(len);
    Fubb[MAX_MSG_SIZE - 1] = '\0';
    printf("client received reply from server: %s\n", Fubb);


    close(Sock_fd);
    Sock_fd = -1;

    return 0;
}

// ./client ::1 halo