//Project 1
//COMP 4320
//Professor Alvin Lim
//Group 2

/*
 * File handling adapted from http://www.fundza.com/c4serious/fileIO_reading_all/
 */

#include <stdlib.h>
#include <printf.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define fileName "testFile.txt"
#define PACKET_SIZE 128

int clientSock;
struct sockaddr_in client;
char* buffer;
long numBytes;
int numPackets;


int readFile();
int segmentAndSend();
int computeChecksum();
int gremlinFunc();

int main(void) {

    readFile();
    segmentAndSend();





    /*client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(12345); //Should this be the FTP port?

    struct hostent* server = gethostbyname("NULL");

    int clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    bind(clientSock, (struct sockaddr*)&client, sizeof(client));

    close(clientSock);*/
    return 0;
}

int clientProcess() {

    return 0;
}

int readFile() {

    //Read file and put in buffer
    FILE* file = fopen(fileName, "r");

    //Check if file is successfully created
    if (file == NULL) {
        printf("Couldn't find specified file...Closing program");
        return 0;
    }

    //Gets number of bytes for file
    fseek(file, 0L, SEEK_END);
    numBytes = ftell(file);
    printf("%ld\n", numBytes);

    //Resets file position indicator to beginning of file
    fseek(file, 0L, SEEK_SET);

    //Allocate memory for the buffer to hold file text
    buffer = (char*)malloc(numBytes);
    if (buffer == NULL) {return 0;} //Memory error checking

    //Put file contents into created character array buffer
    fread(buffer, sizeof(char), numBytes, file);
    fclose(file);
    printf("The file contents are: \n\n%s", buffer);

    //Free memory set for buffer, do this after file is successfully sent to server
    free(buffer);

    return 0;
}

int segmentAndSend(){

    //Determine the number of packets that will need to be sent to the server
    numPackets = (int)numBytes/PACKET_SIZE;

    /*
     * If the number of packets isn't evenly divisible by packet size,
     * we need to send one extra packet that's partially filled with NULL values.
     */
    if (numBytes%PACKET_SIZE > 0) {numPackets++; int fillNull = 1;}
    printf("\n\n\n%d", numPackets);

    //Main loop for creating and sending segments/packets
    int altBit;
    for (int i = 0; i < numPackets; i++) {

        if (i % 2 == 1) {altBit = 1;}
        else {altBit = 0;}

        computeChecksum();

        char* currPacket = (char*)malloc(PACKET_SIZE);

        gremlinFunc();
    }

    return 0;
}

int computeChecksum() {

    return 0;
}

int gremlinFunc() {

}

