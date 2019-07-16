//Project 1
//COMP 4320
//Professor Alvin Lim
//Group 2

#include <printf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{

    printf("Test!");
    return 0;
}

//Internet address structure
struct int_addr
{
    unsigned int s_addr;
};