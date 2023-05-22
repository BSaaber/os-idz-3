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
char* sleep_hotel_stage =      "slph\0";
char* sleep_skameika_stage =   "slps\0";
char* sit_request =            "sitt\0";
int MESSAGE_SIZE = 4;
int DAY_LENGTH = 3;
int CLIENTS_AMOUNT = 10;

int main(int argc, char *argv[])
{




    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */

    srand(time(NULL));

    if (argc != 7)    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Hotel server IP> <Echo port> <Clients amount> <Day length (seconds)> <Skameika server IP> <Echo port>\n",
                argv[0]);
        exit(1);
    }


    CLIENTS_AMOUNT = atoi(argv[3]);

    DAY_LENGTH = atoi(argv[4]);

    //------------------  Create Hotel connection  --------------------------

    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Use given port, if any */


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

    //------------------------------------------------------------------------



    //------------------  Create Skameika connection  ------------------------

    int sock_skameika;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr_skameika; /* Echo server address */
    unsigned short echoServPort_skameika;     /* Echo server port */
    char *servIP_skameika;                    /* Server IP address (dotted quad) */

    servIP_skameika = argv[5];             /* First arg: server IP address (dotted quad) */
    echoServPort_skameika = atoi(argv[6]); /* Use given port, if any */


    /* Create a reliable, stream socket using TCP */
    if ((sock_skameika = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr_skameika, 0, sizeof(echoServAddr_skameika));     /* Zero out structure */
    echoServAddr_skameika.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr_skameika.sin_addr.s_addr = inet_addr(servIP_skameika);   /* Server IP address */
    echoServAddr_skameika.sin_port        = htons(echoServPort_skameika); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr_skameika, sizeof(echoServAddr_skameika)) < 0)
        DieWithError("connect() failed");

    //------------------------------------------------------------------------

    char* clients[CLIENTS_AMOUNT];
    int clients_wait_time[CLIENTS_AMOUNT];

    for (int i = 0; i < CLIENTS_AMOUNT; ++i) {
        // at the start, all clients just want to go to the hotel
        clients[i] = rent_request;
        clients_wait_time[i] = 0;
    }

    for(; ;) {
        // for each client
        for (int i = 0; i < CLIENTS_AMOUNT; ++i) {
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
                    printf("%s\n", echoBuffer);      /* Print the echo buffer */

                    // case success - client rent a room
                    if (!strcmp(echoBuffer, success_response)) {
                        clients[i] = sleep_hotel_stage;
                        clients_wait_time[i] = 1 + rand() % 4; //  sleep from 1 to 5 * DAY_LENGTH seconds (+ process time)
                        printf("horaay! i rent a room! now i go to sleep for %d days\n", clients_wait_time[i]);
                    } else if (!strcmp(echoBuffer, error_response)) { // case there is no space in hotel - go sit on a skameika
                        printf("oh no, all rooms are allocated! now i go to sleep on skameika\n");
                        clients[i] = sit_request;
                    }
                }
            } else if (!strcmp(clients[i], sleep_hotel_stage) || !strcmp(clients[i], sleep_skameika_stage)) { // case 2 - sleep on skameika or in a hotel
                printf("client sleeps\n");
                clients_wait_time[i] -= 1;
                if (clients_wait_time[i] == 0) {
                    printf("client woke up\n");
                    if (!strcmp(clients[i], sleep_hotel_stage)) { // woke up in hotel -> go free
                        printf("i will free a room tomorrow\n");
                        clients[i] = free_request;
                    } else if (!strcmp(clients[i], sleep_skameika_stage)) { // woke up on skameika -> go rent
                        printf("i will try to rent a room tomorrow\n");
                        clients[i] = rent_request;
                    }
                }
            } else if (!strcmp(clients[i], sit_request)) {
                printf("client goes to skameika\n");

                if (send(sock_skameika, clients[i], strlen(clients[i]), 0) != strlen(clients[i]))
                    DieWithError("send() sent a different number of bytes than expected");

                totalBytesRcvd = 0;
                while (totalBytesRcvd < MESSAGE_SIZE) {
                    /* Receive up to the buffer size (minus 1 to leave space for
                    a null terminator) bytes from the sender */
                    if ((bytesRcvd = recv(sock_skameika, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
                        DieWithError("recv() failed or connection closed prematurely");
                    totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
                    echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
                    printf("message, that client %d recieved: ", i);
                    printf("%s\n", echoBuffer);      /* Print the echo buffer */

                    // case success - client rent a room
                    if (strcmp(echoBuffer, success_response)) {
                        printf("WHAT? I CAN NOT SIT ON SKAMEIKA? THIS IS STRANGE!\n");
                        exit(-1);
                    }
                    printf("i got to the skameika.\n");
                }



                clients[i] = sleep_skameika_stage;
                clients_wait_time[i] = 1 + rand() % 4; //  sleep from 1 to 5 * DAY_LENGTH seconds (+ process time)
                printf("I am at skameika now. I go to sleep on skameika for %d days\n", clients_wait_time[i]);
            } else if (!strcmp(clients[i], free_request)) {
                printf("client goes to free a room\n");

                if (send(sock, clients[i], strlen(clients[i]), 0) != strlen(clients[i]))
                    DieWithError("send() sent a different number of bytes than expected");

                totalBytesRcvd = 0;
                while (totalBytesRcvd < MESSAGE_SIZE) {
                    /* Receive up to the buffer size (minus 1 to leave space for
                    a null terminator) bytes from the sender */
                    if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
                        DieWithError("recv() failed or connection closed prematurely");
                    totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
                    echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
                    printf("message, that client %d recieved: ", i);
                    printf("%s\n", echoBuffer);      /* Print the echo buffer */

                    // case success - client rent a room
                    if (strcmp(echoBuffer, success_response)) {
                        printf("WHAT? I CAN NOT FREE A ROOM? THIS IS STRANGE!\n");
                        exit(-1);
                    }
                    printf("i freed a room, i will try to rent a room tomorrow");
                    clients[i] = rent_request;
                }
            }
            printf("\n\n");
            sleep(1);
        }
        sleep(DAY_LENGTH);
        printf("------------------\n\n\n");
    }

    close(sock);
    exit(0);
}
