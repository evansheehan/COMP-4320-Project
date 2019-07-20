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

#define fileName "testFile.txt"

int clientSock;
struct sockaddr_in client;

int main(void)
{
    //Read file and put in buffer
    //Adapted from http://www.fundza.com/c4serious/fileIO_reading_all/
    FILE* file = fopen(fileName, "r");

    //Check if file is successfully created
    if (file == NULL) {
        printf("Couldn't find specified file...Closing program");
        return 1;
    }

    //Gets number of bytes for file
    fseek(file, 0L, SEEK_END);
    long numBytes = ftell(file);
    printf("%d", numBytes);

    //Resets file position indicator to beginning of file
    fseek(file, 0L, SEEK_SET);

    //char* buffer = (char*)calloc(numBytes, sizeof(char));










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