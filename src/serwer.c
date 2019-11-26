#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void chat(int sockfd)
{
    char buff[MAX];
    int n;

    while (1)
    {
        bzero(buff, MAX);

        read(sockfd, buff, sizeof(buff));

        printf("from client: %s\t to client : ", buff);
        bzero(buff, MAX);
        n = 0;

        while ((buff[n++] = getchar()) != '\n')
            ;

        write(sockfd, buff, sizeof(buff));

        if (strncmp("exit", buff, 4) == 0)
        {
            printf("server exit...\n");
            break;
        }
    }
}

int main()
{
    struct sockaddr_in servaddr, cli;

    int socket_fd, connection_fd, length;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        printf("socket not created \n");
        exit(0);
    }
    else
        printf("socket created \n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(socket_fd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind fail\n");
        exit(0);
    }
    else
        printf("socket binded..\n");

    if ((listen(socket_fd, 5)) != 0)
    {
        printf("listen failed\n");
        exit(0);
    }
    else
        printf("server listening \n");
    length = sizeof(cli);

    connection_fd = accept(socket_fd, (SA *)&cli, &length);
    if (connection_fd < 0)
    {
        printf("client accept fail \n");
        exit(0);
    }
    else
        printf("server acccepted client\n");

    chat(connection_fd);

    close(socket_fd);
}