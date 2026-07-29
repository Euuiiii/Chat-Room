#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg){
    perror(msg);
    // perror() is an inbuilt function in <stdio.h> that displays a custom message along with the error description of the code stored in the system variable error.
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        fprintf(stderr, "Port not provided, program terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[256]; 

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("Error opening socket");
    }

    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        error("Error setting socket options");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding failed");
    }

    if(listen(sockfd, 5))
    {
        error("Error on listen");
    }

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if(newsockfd < 0)
    error("Error on Accept");

    while(1)
    {
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if(n<0){
            error("Error on reading.");
        }

        if(n == 0)
        {
            printf("Client discounnected \n");
            break;
        }
        printf("Client : %s\n", buffer);

        bzero(buffer, sizeof(buffer));
        if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            error("Error reading from stdin");
        }

        buffer[strcspn(buffer, "\n")] = 0;

        n = write(newsockfd, buffer, strlen(buffer));
        if(n < 0)
        {
            error("Error on Writing");
        }
        
        int i = strncasecmp("Bye", buffer, 3);

        if(i == 0)
        {
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
