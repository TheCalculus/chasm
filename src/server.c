#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../include/templator_parser.h"

#define IP_ADDR "127.0.0.1"
#define PORT    8888
#define MAX_BUFFER_SIZE 2048

int socket_fd = 0;

void error(int expression, char* err) {
    if (expression >= 0) return;
    perror(err);
    exit(1);
}

void handleClient(int client_fd, Parser* parser) {
    char buffer[MAX_BUFFER_SIZE];

    int bytesRecv = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    error(bytesRecv, "receive failed");
    buffer[bytesRecv] = '\0';

    printf("received %d bytes from client:\n%s\n", bytesRecv, buffer);

    const char* header = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "\r\n";
    // const char* content = "<h1>Hello World!</h1>";
    size_t out;
    const char* content = parseTreeToHTML(&parser->nodes[0], &out, 1024);

    char response[MAX_BUFFER_SIZE];
    snprintf(response, sizeof(response), "%s%s", header, content);

    int bytesSent = send(client_fd, response, strlen(response), 0);
    error(bytesSent, "send failed");
    printf("sent %d bytes to client\n", bytesSent);

    close(client_fd);
}

void signalHandler(int signum) {
    printf("socket interrupted\n");
    close(socket_fd);
    exit(signum);
}

int webserver(Parser* parser) {
    int client_fd;
    struct sockaddr_in server, client;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    error(socket_fd, "failed to create socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP_ADDR);
    server.sin_port = htons(PORT);

    error(bind(socket_fd, (struct sockaddr*)&server, sizeof(server)), "failed to bind socket");

    error(listen(socket_fd, 3), "listen failed");

    printf("server is listening on %s:%d\n", IP_ADDR, PORT);

    signal(SIGINT, signalHandler);

    while (1) {
        socklen_t c = sizeof(client);
        client_fd = accept(socket_fd, (struct sockaddr*)&client, &c);
        error(client_fd, "accept failed");

        handleClient(client_fd, parser);
    }

    close(socket_fd);

    return 0;
}
