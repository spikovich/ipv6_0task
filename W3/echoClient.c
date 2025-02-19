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
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "settings.h"

char Buff[MAX_MSG_SIZE];             // output message buffer
char Fubb[MAX_MSG_SIZE];             // input message buffer

int Sock_fd = -1;

void cleanup(void) {
    if (Sock_fd != -1) {
        close(Sock_fd);
        Sock_fd = -1;
    }
}

// client code
int main(int argc, const char *argv[]) {
    int len;
    struct sockaddr_un address;    // this is for local sockets
    int result;

    // Check for correct number of arguments
    if (argc < 3) {
        printf("Usage: %s <socket_name> <message>\n", argv[0]);
        printf("client error: both socket name and message must be specified.\n");
        return 1;
    }

    atexit(cleanup);

    // create unnamed socket
    Sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    _runSafe(Sock_fd);

    // set server socket name
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, argv[1], sizeof(address.sun_path) - 1);

    // connect to server
    result = connect(Sock_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un));
    _runSafe(result);

    // i add this for debug a rec message
    strncpy(Buff, argv[2], MAX_MSG_SIZE - 1);
    Buff[MAX_MSG_SIZE - 1] = '\0';

    printf("client sending message:%s\n", Buff);
    // write to socket
    len = write(Sock_fd, Buff, strlen(Buff) + 1);
    _runSafe(len);

    // read answer from socket
    len = read(Sock_fd, Fubb, MAX_MSG_SIZE);
    _runSafe(len);
    Fubb[MAX_MSG_SIZE - 1] = '\0';  // nul

    // show server reply to user
    printf("client received reply from server:%s\n", Fubb);

    // close connection
    close(Sock_fd);
    Sock_fd = -1;

    return 0;
}