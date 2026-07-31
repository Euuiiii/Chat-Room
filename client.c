#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#define close closesocket
typedef int socklen_t;

#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#endif

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

#ifdef _WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed\n");
        exit(1);
    }
#endif

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Connection failed");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            error("Error reading from stdin");
        }
        buffer[strcspn(buffer, "\n")] = 0;

        n = send(sockfd, buffer, strlen(buffer), 0);
        if (n < 0)
        {
            error("Error on writing");
        }

        memset(buffer, 0, sizeof(buffer));
        n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n < 0)
        {
            error("Error on reading");
        }

        if (n == 0)
        {
            printf("Server disconnected \n");
            break;
        }

        printf("Server : %s\n", buffer);

        if (strncasecmp("Bye", buffer, 3) == 0)
        {
            break;
        }
    }

    close(sockfd);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}