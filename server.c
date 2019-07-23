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
    char buf[512];

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(12345);

    sd = socket(AF_INET, SOCK_DGRAM, 0);

    bind (sd, (structu sockaddr*)& server, sizeof(server));

    for (;;) {
        n = recv(sd, buf, sizeof(buf), 0);
        buf[n] = '\0';
        printf('Received: %s\n', buf);
    }
    close(sd);
    /*
     * End 'Bare minimum UDP server' from slides
     */

    return 0;
}

int serverProcess() {

    return 0;
}