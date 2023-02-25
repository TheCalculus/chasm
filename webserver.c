#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define  IP_ADDR "127.0.0.1"
#define  PORT    8888

void error(char* err) {
    perror(err);
    exit(1);
}

int webserver() {
    int         socket_fd, client_fd;
    struct      sockaddr_in server, client;
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1)
    {   error("failed to create socket");   }
    
    printf("socket(AF_INET, SOCK_STREAM, 0)\n%s:%d\n\n", IP_ADDR, PORT);

    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_ADDR);
    server.sin_port        = htons(PORT);

    if(bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {   error("failed to bind socket");   }
   
    printf("bind(...) success\n@%d:%d, fd %d\nntoa, hton (%s:%d)\n\n", server.sin_addr.s_addr, 
            server.sin_port, socket_fd, inet_ntoa(server.sin_addr), ntohs(server.sin_port));

    listen(socket_fd, 3);
    int c = sizeof(struct sockaddr_in);

    while (1) {
        if ((client_fd = accept(socket_fd, (struct sockaddr *)&client, (socklen_t *)&c)) < 0)
        {   error("accept failed");   }

        printf("accept(...) success\n@%d:%d, fd %d\nntoa, hton (%s:%d)\n\n", client.sin_addr.s_addr,
                client.sin_port, client_fd, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        char buffer[1024];
        if ((recv(client_fd, buffer, sizeof(buffer), 0)) < 0)
        {   error("receive failed");   }

        printf("recv(...)ed from client %s\n", buffer);

        char* response = "HTTP/1.1 200 OK\r\n\r\n<!doctype html><h1>Hello World!</h1>";
        int   bytes;

        if ((bytes = send(client_fd, response, strlen(response), 0)) < 0)
        {   error("send failed");   }

        printf("send(...)/sent %d bytes to client\n%s\n\n", bytes, response);

        close(client_fd);
    }

    close(socket_fd);

    return 0;
}
