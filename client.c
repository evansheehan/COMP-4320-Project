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
int dmgPktProb;
int lostPktProb;
int sockfd;
struct sockaddr_in serverAddr;

socklen_t addr_size;


int readFile();
int segmentAndSend();
unsigned char calculateChecksum(char sourcePacket[], unsigned char length);
int gremlinFunc(char sourcePackets[]);

int main(int argc, char** argv) {

    srand(time(NULL));

    dmgPktProb = atoi(argv[1]);
    lostPktProb = atoi(argv[2]);

    printf("First two arguments you type:\t%d\t%d\n\n\n", dmgPktProb, lostPktProb);
    
  char buffer[1024];

  sockfd = socket(PF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(5566);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  strcpy(buffer, "Hello Server\n");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  printf("[+]Data Send: %s", buffer);





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

int segmentAndSend(char* mainBuffer){

    //Determine the number of packets that will need to be sent to the server
    numPackets = (int)numBytes/(PACKET_SIZE - 3); //minus 3 currently for header
    currentBufferPos = 0;
    char voidPacket[128];

    /*
     * If the number of packets isn't evenly divisible by packet size,
     * we need to send one extra packet that's partially filled with NULL values.
     */
    if (numBytes%PACKET_SIZE > 0) {numPackets++; int fillNull = 1;}
    printf("\n\n\n%d", numPackets);

    //Main loop for creating and sending segments/packets
    char altBit;

    

    for (int i = 0; i < numPackets; i++) {
        if (i % 2 == 1) {altBit = '1';}
        else {altBit = '0';}

         //For now the packet-header structure will be 1 byte Ack, 1 Byte Seq, 
        char currPacket[PACKET_SIZE];
        currPacket[0] = '0'; //just init value for checksum
        currPacket[1] = '0'; //just init value for Ack/Nck
        currPacket[2] = altBit;
        for (int i = 3; i < 128; i++) {
            if (mainBuffer[currentBufferPos] == '\0') {
                currPacket[i] = '\0';
            }
            else {
                currPacket[i] = (char)mainBuffer[currentBufferPos++];
            }          
        }
       
        //calculate checksum and gremlin in this bish
        currPacket[0] = calculateChecksum(currPacket, 128);
        gremlinFunc(currPacket);
        sendto(sockfd, currPacket, 1024, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        sendto(clientSock, "hello!", 128, 0, (struct sockaddr *)&server, sizeof(server));
        printf("SENT BOY");
        
        recv(clientSock, voidPacket, sizeof(voidPacket), 0);
        printf("%s", voidPacket);

        

        
    }

    return 0;
}

//We are going to keep it simple
unsigned char calculateChecksum(char sourcePacket[], unsigned char length)
 {
     unsigned char count;
     unsigned int checkSum = 0;
     
     for (count = 1; count < length; count++)
         checkSum += sourcePacket[count];
     checkSum = -checkSum;
     return (checkSum & 0xFF);
 }

int gremlinFunc(char sourcePacket[]) {

    int lowerRand = 0;
    int upperRand = 100;

    int corruptBool = 0;
    int dropBool = 0;

    int randNum = (rand() % (upperRand - lowerRand + 1) + lowerRand);

    if (randNum <= dmgPktProb) {corruptBool = 1;}
    if (randNum <= lostPktProb) {dropBool = 1;}

    printf("\n\n\n\nCorrupt packet?\t%d\nLose packet?\t%d\n", corruptBool, dropBool);

    /*upperRand = PACKET_SIZE - 1;
    randNum = (rand() % (upperRand - lowerRand + 1) + lowerRand);*/

    if (corruptBool) {

        //Determine number of bytes that will be corrupted in this packet
        int bytesToCorrupt;
        randNum = (rand() % (upperRand - lowerRand + 1) + lowerRand);
        if (randNum <= 10) {bytesToCorrupt = 3;}
        else if (randNum <= 30) {bytesToCorrupt = 2;}
        else {bytesToCorrupt = 1;}

        //Find which index/byte to corrupt
        for (int i = 0; i < bytesToCorrupt; i++) {
            printf("Corrupting %d bytes...\n", bytesToCorrupt);
            int indexToCorrupt;
            upperRand = PACKET_SIZE - 1;
            indexToCorrupt = (rand() % (upperRand - lowerRand + 1) + lowerRand);

            //Set index to random byte value
            int setVal;
            upperRand = 255;
            setVal = randNum = (rand() % (upperRand - lowerRand + 1) + lowerRand);
            int previousValue = sourcePacket[indexToCorrupt];
            sourcePacket[indexToCorrupt] = setVal;
            printf("Byte number %d of value %d has been changed to %d\n", indexToCorrupt, previousValue
                    , setVal);
        }
    }

    if (dropBool) {

    }
}

