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
#include <string.h>

#define fileName "testFile.txt"
#define PACKET_SIZE 128

int clientSock;
struct sockaddr_in server;
char* buffer;
long numBytes;
int numPackets;
int currentBufferPos;


int readFile();
int segmentAndSend();
unsigned char calculateChecksum(unsigned char *sourcePacket, unsigned char length);
int gremlinFunc();

int main(void) {

    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(12345); 

    int clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    bind(clientSock, (struct sockaddr*)&server, sizeof(server));


    readFile();


    close(clientSock);
    return 0;
}

int serverProcess() {

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

    segmentAndSend(buffer);

    //Free memory set for buffer, do this after file is successfully sent to server
    free(buffer);

    return 0;
}

int segmentAndSend(char* mainBuffer[]){

    //Determine the number of packets that will need to be sent to the server
    numPackets = (int)numBytes/(PACKET_SIZE - 3); //minus 3 currently for header
    currentBufferPos = 0;
    char *voidPacket[128];

    /*
     * If the number of packets isn't evenly divisible by packet size,
     * we need to send one extra packet that's partially filled with NULL values.
     */
    if (numBytes%PACKET_SIZE > 0) {numPackets++; int fillNull = 1;}
    printf("\n\n\n%d", numPackets);

    //Main loop for creating and sending segments/packets
    char* altBit;

    

    for (int i = 0; i < numPackets; i++) {
        if (i % 2 == 1) {altBit = "1";}
        else {altBit = "0";}

         //For now the packet-header structure will be 1 byte Ack, 1 Byte Seq, 
        char* currPacket[PACKET_SIZE/ sizeof(char)];
        currPacket[0] = "0"; //just init value for checksum
        currPacket[1] = "0"; //just init value for Ack/Nck
        currPacket[2] = altBit;
        for (int i = 3; i < 128; i++) {
            if (strcmp(mainBuffer[currentBufferPos], "\0") == 1) {
                currPacket[i] = "\0";
            }
            else {
                currPacket[i] = mainBuffer[currentBufferPos++];
            }          
        }
        //calculate checksum and gremlin in this bish
        currPacket[0] = calculateChecksum(currPacket, 128);
        gremlinFunc(currPacket);

        sendto(clientSock, currPacket, 128, 0, (struct sockaddr *) &server, sizeof(server));
        printf("SENT BOY");
        recv(clientSock, voidPacket, sizeof(voidPacket), 0);

        

        
    }

    return 0;
}

//We are going to keep it simple
unsigned char calculateChecksum(unsigned char *sourcePacket, unsigned char length)
 {
     unsigned char count;
     unsigned int checkSum = 0;
     
     for (count = 1; count < length; count++)
         checkSum = checkSum + sourcePacket[count];
     checkSum = -checkSum;
     return (checkSum & 0xFF);
 }

int gremlinFunc(char *sourcePacket) {

}

