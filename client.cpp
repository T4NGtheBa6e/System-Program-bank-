#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

constexpr unsigned int SERVER_PORT = 50544;
constexpr unsigned int MAX_BUFFER = 128;
using namespace std;
void saveMoney(int sockfd);
void takeMoney(int sockfd);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error! usage: ./client localhost" << std::endl;
	return 6;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        std::cerr << "socket error" << std::endl;
        return 1;
    }

    struct hostent* server = gethostbyname(argv[1]);
    if (server == nullptr) 
    {
        std::cerr << "gethostbyname, no such host" << std::endl;
        return 2;
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr, 
          server->h_length);
    serv_addr.sin_port = htons(SERVER_PORT);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "connect error: the server does not look running" << std::endl;
        return 3;
    }

    std::string readBuffer (MAX_BUFFER, 0);
    if (read(sockfd, &readBuffer[0], MAX_BUFFER-1) < 0)
    {
        std::cerr << "read from socket failed" << std::endl;
        return 5;
    }
    std::cout << readBuffer << std::endl;

    while(1){

    std::string writeBuffer (MAX_BUFFER, 0);
    std::cout << "What message for the bank? : ";
    getline(std::cin, writeBuffer);
    write(sockfd, writeBuffer.c_str(), strlen(writeBuffer.c_str()));
    

    std::string readBuffer (MAX_BUFFER, 0);
    read(sockfd, &readBuffer[0], MAX_BUFFER-1);
    std::cout << readBuffer << std::endl;

     //存取款功能
    if(strcmp((const char *)writeBuffer.c_str(), (const char *)"save") == 0)
    {
        saveMoney(sockfd);

    }
    else if(strcmp((const char *)writeBuffer.c_str(), (const char *)"take") == 0)
    {
        takeMoney(sockfd);
    }
    else
    {
        cout << "command error" << endl;
        break;
     }

    

    }//while{}

    close(sockfd);
    return 0;
}



void takeMoney(int sockfd)
{
        cout << "How much money want to take?: " ;
        std::string moneyBuffer (MAX_BUFFER, 0);
        getline(std::cin, moneyBuffer);
        write(sockfd, moneyBuffer.c_str(), strlen(moneyBuffer.c_str()));
        std::string Buffer (MAX_BUFFER, 0);
        read(sockfd, &Buffer[0], MAX_BUFFER-1);
        std::cout << Buffer << std::endl;
}

void saveMoney(int sockfd)
{
        cout << "How much money want to save?: " ;
        std::string moneyBuffer (MAX_BUFFER, 0);
        getline(std::cin, moneyBuffer);
        write(sockfd, moneyBuffer.c_str(), strlen(moneyBuffer.c_str()));

        std::string Buffer (MAX_BUFFER, 0);
        read(sockfd, &Buffer[0], MAX_BUFFER-1);
        std::cout << Buffer << std::endl;

}
