/*
*********Project Participants***********
313327579
208849620
*/
// define constants that will be in often usage
#define myPort 5093
#define ipAdd "172.21.240.1"
#define SIZE 65536
//Import the relevant libraries
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <time.h>
#include <errno.h>
#include <string.h>

/*Now we will start with the main Function
  this is where the entire process will happen
*/

int main(int argc, char const* argv[]) {

    int finished = 0;

    int mySocket = 0;

    struct sockAddIn server_address;

    /*this for loop will inform the user that the socket is open
    * However, if an error accours, en arror will be printed
    * We will also limit the For loop for 5 due to packet withold
    *  stable restrictions that will prevent and unPlanned leak
    *  (Obviously besides that planned ones regarding Congestion Control)
    */

    for (int i = 0; i < 5; i++) {
        char buffer[SIZE] = { 0 };
        sSocket = socket(AF_INET, SOCK_STREAM, 0)
            if (sSocket < 0)
            {
                printf("Error \n");
                return -1;
            }

        /* On this block of code we will atrribute the address to the Inhereted
        * network ancestor. a print message will be printed if an error Accoured.
        */

        bzero(&server_address, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(myPort);
        if (inet_pton(AF_INET, (const char*)ipAdd, &server_address.sin_addr) <= 0)
        {
            printf("Error! \n");
            return -1;
        }

        /*now we will establish the connection (aka initiate the sending)
        * we will also print a message that will notify the user that a connection as established
        * also, we will let the user, as always, know if an error was accoured
        */
        printf("Connection is being established: \n");
        if (connect(mySocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        {
            printf("Connection Error!");
            return -1;
        }

        printf("Connected! \n");

        /* now as we succesfully established the connection, we will begin to send packets.
        * we will inform the user that packets (or a packet) are being sent.
        * as always, if there is an error we will let the user now by printing it
        */

        printf("Seding has begun: \n");


        FILE* myFile;
        myFile = fopen("1gb.txt", "r");
        if (myFile == NULL) {
            perror("File Error! \n");
            return 2;
        }

        /* on this point the sending is actually beginning using a WHILE LOOP:
        * we will first set everything to zero, then we begin to read packts from the user
        * through the proccess we will ontify the user of :
        * 1. the file that is being sent.
        * 2. if the connection was successfuly established.
        * 3. print if there was an error!
        */

        printf("Sending file %d...\n", i + 1);
        int b = 0;
        int bytesSent = 0;
        int count = 0;
        int amountSeg = 0;
        while ((b = fread(buffer, 1, sizeof(buffer), myFile)) > 0) {
            int bytesSent = send(mySocket, buffer, b, 0);
            if (-1 == bytesSent)
            {
                printf("Error! \n %d couldn't be sent", errno);
            }
            else if (0 == bytesSent)
            {
                printf("Connection was killed too early!");
            }
            else if (b > bytesSent)
            {
                printf("Not everything was sent. \n sent: %d \n needed: %d", bytesSent, b);
            }
            else
            {
                printf("Message sent!");
            }
            count += bytesSent;
            amountSeg += 1;
        }
        printf("there are %d bytes from the file named %d\n Seg: %d\n", count, i + 1, amountSeg);
        fclose(myFile);
        close(mySocket);
    }

    /* Here we begin the "CC" Process. we will also notify the user the the process has began.
    */
    printf("CC activated! \n");
    for (int i = 0; i < 5; i++) {
        mySocket = 0;
        char buffer[SIZE] = { 0 };
//Hello
        sSocket = socket(AF_INET, SOCK_STREAM, 0)
            if (sSocket < 0) {
                printf("Socket Error!\n");
                return -1;
            }

        bzero(&server_address, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(myPort);
        if (inet_pton(AF_INET, (const char*)ipAdd, &server_address.sin_addr) <= 0) {
            printf("Error! \n");
            return -1;
        }
        /* here we Change the Algorithm to 'Reno' and obviously apply change in
        *  congestion
        */
        strcpy(buffer, "reno");
        if (setsockopt(mySocket, IPPROTO_TCP, TCP_CONGESTION, buffer, strlen(buffer)) != 0) {
            perror("setsockopt");
            return -1;
        }
        socklen_t len = sizeof(buffer);
        if (getsockopt(mySocket, IPPROTO_TCP, TCP_CONGESTION, buffer, &(len)) != 0) {
            perror("getsockopt");
            return -1;
        }
        printf("CC is now: %s \n", buffer);

        printf("Connection is being established: \n");
        if (connect(mySocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            printf("Connection Error! \n");
            return -1;
        }
        /*Now we will open the file in rb mode and begin a relativley similar process to the one
         that was seen in lines ~86. we will notify the user according to the outcome of the process.*/

        FILE* myFile;
        printf("Sending:%d \n", i + 1);
        myFile = fopen("1gb.txt", "rb");
        if (myFile == NULL)
        {
            perror("File Error!");
            return 2;
        }
        int byte = 0;
        int finishedBytes = 0;
        int count = 0;
        int totalSegment = 0;
        bzero(buffer, SIZE);
        while ((byte = fread(buffer, 1, sizeof(buffer), myFile)) > 0) {
            finishedBytes = send(mySocket, buffer, byte, 0);

            if (-1 == finishedBytes)
            {
                printf("File Error!:  %d \n", errno);
            }
            else if (0 == finishedBytes)
            {
                printf("Session terminated too early \n");
            }
            else if (byte > finishedBytes)
            {
                printf("%d bytes of out %d were sent. \n", finishedBytes, byte);
            }
            else
            {
                printf("Message sent! \n", finishedBytes);
            }
            count += finishedBytes;
            totalSegment += 1;
        }
        /*Here we are almost finished, we will let the user know how many packets
         were sent and following by that we will close the Large file and the socket. */

        printf("Send %d bytes of file %d by %d segments.\n", count, i + 1, totalSegment);

        fclose(myFile);

        close(mySocket);
    }

    /*Return 0 as a convention in C.*/

    return 0;
}