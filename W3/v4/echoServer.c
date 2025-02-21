/* ========================================================================== */
/*                                                                            */
/*   echoServer.c                                                             */
/*   (c) 2012 Tomas Palenik                                                   */
/*                                                                            */
/*   Server opens a LOCAL-type socket and listens for a message from client.  */
/*              The message should be a lo-case string.                       */
/*              After receiving the message,                                  */
/*              the server replies with a response message - client's message */
/*              converted to upper case letters.                              */
/* ========================================================================== */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "settings.h"
#include <string.h>
char Buff[MAX_MSG_SIZE];
int server_sockfd = -1;
int client_sockfd = -1;

void cleanup(void) {
    if (server_sockfd != -1) {
        close(server_sockfd);
        server_sockfd = -1;
    }
    if (client_sockfd != -1) {
        close(client_sockfd);
        client_sockfd = -1;
    }
}

int strToUpper(char *str, int size) {
    int i = 0;
    while (*str != '\0' && i++ < size) {
        *str = toupper(*str);
        str++;
    }
    return i;
}

int main(int argc, const char *argv[]) {
    int len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t client_len;

    if (argc < 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        printf("server error: server IP must be specified.\n");
        return 1;
    }

    atexit(cleanup);


    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    _runSafe(server_sockfd);


    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);  // Port 5000 in network byte order
    inet_pton(AF_INET, argv[1], &server_address.sin_addr);  // Bind to specified IP


    _runSafe(bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)));


    _runSafe(listen(server_sockfd, 5));

    while (1) {
        printf("Server waiting.\n");


        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        _runSafe(client_sockfd);


        len = read(client_sockfd, Buff, MAX_MSG_SIZE);
        _runSafe(len);
        Buff[MAX_MSG_SIZE - 1] = '\0';
        printf("Server received: %s\n", Buff);


        strToUpper(Buff, MAX_MSG_SIZE);
        len = write(client_sockfd, Buff, strlen(Buff) + 1);
        _runSafe(len);


        close(client_sockfd);
        client_sockfd = -1;
    }

    return 0;
}
// tmp/socket or /var/run/ or ~/
// ./server 127.0.0.1