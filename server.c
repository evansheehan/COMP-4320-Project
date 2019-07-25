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
   char buffer[128];
   socklen_t addr_size;
   char checksum = '0';

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   memset(&si_me, '\0', sizeof(si_me));
   si_me.sin_family = AF_INET;
   si_me.sin_port = htons(5566);
   si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

   bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me));
   addr_size = sizeof(si_other);

   int seqNumExpected = 0;

   for (;;) {
   
      recvfrom(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, &addr_size);
   
    
      printf("[+]Data Received: First 48 bytes of current packet:\n");
      printf("Checksum:%d\n", (unsigned int)(unsigned char)buffer[0]);
      printf("ACK:%c\n", buffer[1]);
      printf("Sequence #:%c\n", buffer[2]);
      for (int i = 3; i < 48; i++) {
         printf("%c", buffer[i]);
      }
      printf("\n");
      checksum = calculateChecksum(buffer, 128);
      if (buffer[0] != checksum) {
         printf("Checksum calculated as: %d. Message corrupted, retransmitting...\n\n", (unsigned int)(unsigned char)checksum);
         sendto(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, sizeof(si_other));
      }
      else {
      }
       //seq num check
      /*if (seqNumExpected != buffer[2]) {
         printf("\nSequence number incorrect... expected: %c", seqNumExpected);
         sendto(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, sizeof(si_other));
      }
      else {*/
      buffer[1] = '1';
      sendto(sockfd, buffer, 128, 0, (struct sockaddr*)& si_other, sizeof(si_other));
         /*if ((int)seqNumExpected % 2 == 0) {
            seqNumExpected = '1';
         }
         else {
            seqNumExpected = '0';
         }
      }*/
   }



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