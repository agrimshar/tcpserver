/* sockets.c */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define IP "142.251.33.174" /* google.com ipv4 address */
#define PORT 80 /* http port */
#define BUFFER_SIZE 4096

int main()
{
    int sockfd;
    struct sockaddr_in sockaddr;
    char buffer[BUFFER_SIZE];
    char *message = "HEAD / HTTP/1.0\r\n\r\n";

    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = inet_addr(IP);

    if(connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("Connected to %s:%d\n", IP, PORT);

    if(write(sockfd, message, strlen(message)) < 0) {
        perror("write");
        close(sockfd);
        return -1;
    }

    printf("Sent message:\n%s\n", message);

    memset(buffer, 0, BUFFER_SIZE);

    if(read(sockfd, buffer, BUFFER_SIZE) < 0) {
        perror("read");
        close(sockfd);
        return -1;
    }
    close(sockfd);

    printf("Connection closed\n");

    printf("Received message:\n%s\n", buffer);

    return 0;
}
