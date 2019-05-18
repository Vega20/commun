#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8888
#define BACKLOG 10
#define MAXDATASIZE  2048

int main(int argc, char *argv[])
{
int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("listenfd=%d\n", listenfd);
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT); 
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, BACKLOG);
    printf("======bind success,waiting for client's request======\n");
    struct sockaddr_in peeraddr;
    socklen_t peer_len = sizeof(peeraddr);
    int connfd;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)&peeraddr, &peer_len);
        printf("\n=====================client connect successfully!=====================\n");
        printf("IP = %s:PORT = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        char buf[MAXDATASIZE];
        while(1)
        {
            memset(buf, '\0', MAXDATASIZE/sizeof  (char));
            int recv_length = recv(connfd, buf, MAXDATASIZE/sizeof (char), 0);
            if(recv_length == 0)
            {
                printf("client has closed!\n");
                break;
            }
            printf("client say: ");
            fputs(buf, stdout);
            memset(buf, '\0', MAXDATASIZE/sizeof (char));
            printf("input: ");
            fgets(buf, sizeof(buf), stdin);
            send(connfd, buf, recv_length, 0);
        }
        close(connfd);
        close(listenfd);
        return 0;
}}

