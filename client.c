//Project 1
//COMP 4320
//Professor Alvin Lim
//Group 2

#include <printf.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int clientSock;
struct sockaddr_in client;

int main(void)
{
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_port = htons(12345); //Should this be the FTP port?

    struct hostent* server = gethostbyname("NULL");

    int clientSock = socket(AF_INET, SOCK_DGRAM, 0);
    bind(clientSock, (struct sockaddr*)&client, sizeof(client));

    close(clientSock);
    return 0;
}

//Internet address structure
struct int_addr
{
    unsigned int s_addr;
};