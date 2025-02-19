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
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include "settings.h"

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

// converts string to upper case
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
    int client_len;

    // local sockets
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;

    if (argc < 2) {
        printf("server error: no socket name specified.\n");
        return 1;
    }
    atexit(cleanup);

    // remove old sockets with same name as specified
    unlink(argv[1]);

    // create socket
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    _runSafe(server_sockfd);

    // bind a name to socket
    memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, argv[1], sizeof(server_address.sun_path) - 1);

    _runSafe(bind(server_sockfd, (struct sockaddr *)&server_address,
                  sizeof(struct sockaddr_un)));

    // start listening on the named socket
    _runSafe(listen(server_sockfd, 5));

    while (1) {
        printf("Server waiting.\n");

        // accept incoming client connection
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,
                               (struct sockaddr *)&client_address,
                               &client_len);
        _runSafe(client_sockfd);

        // read message from client
        len = read(client_sockfd, Buff, MAX_MSG_SIZE);
        _runSafe(len);
        Buff[MAX_MSG_SIZE - 1] = '\0';  // null

        printf("Server received: %s\n", Buff);

        // convert client's message to upper case and reply to client
        strToUpper(Buff, MAX_MSG_SIZE);
        len = write(client_sockfd, Buff, strlen(Buff) + 1);
        _runSafe(len);

        // close temporary socket
        close(client_sockfd);
        client_sockfd = -1;
    }

    // program will never reach this point
    return 0;
}

// tmp/socket or /var/run/ or ~/
