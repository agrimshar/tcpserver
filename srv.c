/* sockets.c */

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8181

int main()
{
    int serverfd, clientfd;
    socklen_t addressLength;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[4096];
    char *message = "Hello, World!\n";

    addressLength = 0;
    memset(&serverAddress, 0, sizeof(serverAddress));
    memset(&clientAddress, 0, sizeof(clientAddress));

    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1) {
        perror("socket");
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("bind");
        close(serverfd);
        return -1;
    }

    if(listen(serverfd, 5) == -1) {
        perror("listen");
        close(serverfd);
        return -1;
    }

    printf("Listening on port %d\n", PORT);

    clientfd = accept(serverfd, (struct sockaddr *)&serverAddress, &addressLength);

    if (clientfd == -1) {
        perror("accept");
        close(serverfd);
        return -1;
    }

    printf("Accepted connection from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
    read(clientfd, buffer, sizeof(buffer));
    printf("Received message:\n%s\n", buffer);

    write(clientfd, message, strlen(message));
    printf("Sent message:\n%s\n", message);

    close(clientfd);
    close(serverfd);

    return 0;
}
