#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define  IP_ADDR "192.168.0.27"
#define  PORT    8888

void error(int expression, char* err) {
    if (expression >= 0) return;
    perror(err);
    exit(1);
}

int webserver() {
    int         socket_fd, client_fd;
    struct      sockaddr_in server, client;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    error(socket_fd, "failed to create socket");

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_ADDR);
    server.sin_port        = htons(PORT);

    error(bind(socket_fd, (struct sockaddr *)&server, sizeof(server)), "failed to bind socket");

    listen(socket_fd, 3);
    int c = sizeof(struct sockaddr_in);

    while (1) {
        error((client_fd = accept(socket_fd, (struct sockaddr *)&client, (socklen_t *)&c)), "accept failed");

        char buffer[1024];

        int  bytesRecv;
        int  bytesSent;

        error((bytesRecv = recv(client_fd, buffer, sizeof(buffer), 0)), "receive failed");
        printf("received %d bytes from client\n\n%s", bytesRecv, buffer);

        char* header   = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n";
        char* content  = "<h1>Hello World!</h1>";
        char* response = malloc(strlen(header) + strlen(content) + 1);
       
        strcpy(response, header);
        strcat(response, content);
        
        error((bytesSent = send(client_fd, response, strlen(response), 0)), "send failed");
        printf("sent %d bytes to client\n%s\n\n", bytesSent, response);

        close(client_fd);
        free(response);
    }

    close(socket_fd);

    return 0;
}
