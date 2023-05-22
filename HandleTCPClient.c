#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <string.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket, int* free_space)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */
    char* success_response = "okay\0";
    char* error_response =   "fail\0";
    char* rent_request = "rent\0";
    char* free_request = "free\0";
    char* response;

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");
    printf("log: free space - %d", *free_space);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        if (!strcmp(free_request, echoBuffer)) {
            *free_space++;
            response = success_response;
            printf("log: freeing room. space now - %d", *free_space);
        } else if (!strcmp(rent_request, echoBuffer)) {
            if (*free_space > 0) {
                *free_space -= 1;
                response = success_response;
                printf("log: allocating room. space now - %d", *free_space);
            } else {
                response = error_response;
                printf("log: all rooms are allocated. free space - %d", *free_space);
            }
        }
        /* Echo message back to client */
        if (send(clntSocket, response, strlen(response), 0) != strlen(response))
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed - more data to recv");
    }

    close(clntSocket);    /* Close client socket */
    printf("log: connection closed");
}

