//
// Created by Evan Sheehan on 2019-07-16.
//

#include <stdlib.h>
#include <printf.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>

#define fileName "testFile.txt"

#define PACKET_SIZE 128
char calculateChecksum(char sourcePacket[], unsigned int length);

int main(void) {
  int sockfd;
  struct sockaddr_in si_me, si_other;
  char* buffer = (char*)malloc(PACKET_SIZE);
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&si_me, '\0', sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(5566);
  si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me));
  addr_size = sizeof(si_other);

  char seqNumExpected = '0';
  int packetsReceived = 0;
  int finished = 0;

  FILE* file  = fopen("outputFile.txt",  "w+");

  for (;;) {

    recvfrom(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, &addr_size);

    
   
    
    char checksumVal = calculateChecksum(buffer, 128);
    if (checksumVal == buffer[0] && seqNumExpected == buffer[2]) {
        printf("Checksum for packet matches! Sending back ACK\n");
        buffer[1] = '1';
        for (int i = 3; i < 128; i++) {
           fprintf(file, "%c", buffer[i]);          
        }

        if (seqNumExpected == '1') seqNumExpected = '0';
        else seqNumExpected = '1';
        packetsReceived++;
        if (buffer[127] == '\0') finished = 1;
    }
    else {
        printf("\nChecksum for packet does not match, sending NCK\n");
    }
    printf("First 48 bytes of current packet:\n");
       for (int i = 3; i < 48; i++) {
         printf("%c", buffer[i]);
    }
    printf("\nChecksum:%d\n", (unsigned int)(unsigned char)buffer[0]);
    printf("ACK:%c\n", buffer[1]);
    printf("Sequence #:%c\n", buffer[2]);
  
    printf("\n\n");
    sendto(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, sizeof(si_other));
    if (finished == 1) break;
    
  }
  char* finalAck = "PUT Operation Successful\n";
  printf("\n\n Packets Received: %d\n", packetsReceived);
  sendto(sockfd, finalAck, 128, 0, (struct sockaddr*)& si_other, sizeof(si_other));



    return 0;
}
char calculateChecksum(char *sourcePacket, unsigned int length)
 {
     unsigned char count;
     unsigned int checkSum = 0;
     
     for (count = 1; count < length; count++)
         checkSum += sourcePacket[count];
     return (checkSum & 0xFF);
 }
