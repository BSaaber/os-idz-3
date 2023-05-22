#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

char* success_response =       "okay\0";
char* error_response =         "fail\0";
char* rent_request =           "rent\0";
char* free_request =           "free\0";
char* sleep_stage =            "slep\0";
char* sit_request =            "sitt\0";
int MESSAGE_SIZE = 4;
int DAY_LENGTH = 3;

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */

    srand(time(NULL));

    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Port>\n",
                argv[0]);
        exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */

    if (argc == 4)
        echoServPort = atoi(argv[2]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    int clients_amount = 30;
    char* clients[clients_amount];
    int clients_wait_time[clients_amount];

    for (int i = 0; i < clients_amount; ++i) {
        // at the start, all clients just want to go to the hotel
        clients[i] = rent_request;
        clients_wait_time[i] = 0;
    }

    for(; ;) {
        // for each client
        for (int i = 0; i < clients_amount; ++i) {
            printf("processing client %d\n", i);


            // case 1 - client i want to try to rent a room
            if (!strcmp(clients[i], rent_request)) {
                printf("client tries to rent a room\n");
                /* Send the string to the server */
                if (send(sock, clients[i], strlen(clients[i]), 0) != strlen(clients[i]))
                    DieWithError("send() sent a different number of bytes than expected");

                /* Receive string back from the server */
                totalBytesRcvd = 0;
                while (totalBytesRcvd < MESSAGE_SIZE) {
                    /* Receive up to the buffer size (minus 1 to leave space for
                    a null terminator) bytes from the sender */
                    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
                        DieWithError("recv() failed or connection closed prematurely");
                    totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
                    echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
                    printf("message, that client %d recieved: ", i);
                    printf("%s", echoBuffer);      /* Print the echo buffer */

                    // case success - client rent a room
                    if (!strcmp(echoBuffer, success_response)) {
                        clients[i] = sleep_stage;
                        clients_wait_time[i] = 1 + rand() % 4; //  sleep from 1 to 5 * DAY_LENGTH seconds (+ process time)
                        printf("horaay! i rent a room! now i go to sleep for %d days\n", clients_wait_time[i]);
                    } else if (!strcmp(echoBuffer, error_response)) { // case there is no space in hotel - go sit on a skameika
                        printf("oh no, all rooms are allocated! now i go to sleep on skameika\n");
                        clients[i] = sit_request;
                    }

                    sleep(DAY_LENGTH);
                }
            } else if (!strcmp(clients[i], sleep_stage)) { // case 2 - sleep on skameika or in a hotel
                printf("client sleeps\n");
                clients_wait_time[i] -= 1;
                if (clients_wait_time[i] == 0) {
                    printf("client woke up\n");
                    printf("i will try to rent a room in future\n");
                    clients[i] = rent_request;
                }
            } else if (!strcmp(clients[i], sit_request)) {
                printf("client goes to skameika\n");
                // todo - request to skameika server
                clients[i] = sleep_stage;
                clients_wait_time[i] = 1 + rand() % 4; //  sleep from 1 to 5 * DAY_LENGTH seconds (+ process time)
                printf("I am at skameika now. I go to sleep on skameika for %d days\n", clients_wait_time[i]);
            }
            printf("\n\n");
            sleep(DAY_LENGTH);
        }
    }

    close(sock);
    exit(0);
}
