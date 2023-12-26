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

#define UNAME_BROADCAST 1
#define NORMAL_MSG 2

#define MAX_MSG_LENGTH 400
#define MAX_UNAME_LENGTH 20

#define servPORT 4444

extern fd_set readFds;
extern int clientSocket;

int join(char* username);
int sendMessage(int clientSocket,char *buff, char *username, int type);
void msgRecvHdlr(int signum);
void rcvMsg();
