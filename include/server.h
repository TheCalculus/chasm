#ifndef SERVER_H
#define SERVER_H

void error(int expression, char* err);
void handleClient(int client_fd, Parser* parser);
int  webserver(Parser* parser);

#endif