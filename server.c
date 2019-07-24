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

#define SERV_PORT 0000
#define PACKET_SIZE 128

int main(void) {

    /*
     * 'Bare minimum UDP server' from slides
     */
    int n, sd;
    struct sockaddr_in server;
    char buf[128];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(12346);

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    bind (sd, (struct sockaddr*)& server, sizeof(server));
    printf("initialiazed");
   /* for (;;) {
        n = recv(sd, buf, sizeof(buf), 0);
        buf[n] = '\0';
        printf("Received: %s\n", buf);
        buf[1] = '1';
        sendto(sd, buf, sizeof(buf), 0, (struct sockaddr *) &server, sizeof(server));
    }*/
    recvfrom(sd, buf, 128, 0, (struct sockaddr*)& server, sizeof(server));
    close(sd);
    /*
     * End 'Bare minimum UDP server' from slides
     */

    return 0;
}

int serverProcess() {

    return 0;
}