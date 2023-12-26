#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <signal.h>


#define servPORT 8080

extern fd_set readFds;
extern int clientSocket;

int join(char* username);
int sendMessage(int clientSocket,char *buff);
void msgRecvHdlr(int signum);
void rcvMsg();
