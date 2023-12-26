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

#define servPORT 4444

#define UNAME_BROADCAST 1
#define NORMAL_MSG 2

#define MAX_MSG_LENGTH 400
#define MAX_UNAME_LENGTH 20

using namespace std;

char buffer[1024];

int sendMessage(int clientSocket, char *buff, char *username, int type)
{
    char message[10 + MAX_MSG_LENGTH + MAX_UNAME_LENGTH];
    if (type == UNAME_BROADCAST)
    {
        strcpy(message, "UNAME:");
    }
    else if (type == NORMAL_MSG)
    {
        strcpy(message, "MSG:");
        strcat(message, username);
        strcat(message, ":");
    }

    strcat(message, buff);
    size_t msgLen = strlen(message);

    int msg = send(clientSocket, &msgLen, sizeof(msgLen), 0);

    if (msg == -1)
    {
        cerr << "[SYSTEM] Error sending message\n";
        return -1;
    }

    msg = send(clientSocket, message, msgLen, 0);

    if (msg == -1)
    {
        cerr << "[SYSTEM] Error sending message data \n";
        return -1;
    }
    else
    {
        //cout << "[SYSTEM] Sent: " << buff << '\n';
    }
}

int initializeSocket(const char *servIP,char *username)
{
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        cerr << "[SYSTEM] Error creating socket\n";
        return -1;
    }
    else
    {
        cout << "[SYSTEM] Socket Created Successfully\n";
    }

    // Set up the address family
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(servPORT);
    serverAddress.sin_addr.s_addr = inet_addr(servIP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        cerr << "[SYSTEM] Error connecting to the server\n";
        close(clientSocket);
        return -1;
    }
    else
    {
        sendMessage(clientSocket,username,NULL,UNAME_BROADCAST); 
        cout << "[SYSTEM] Connected to the server at " << servIP << ":" << ntohs(serverAddress.sin_port) << "\n";
    }

    return clientSocket;
}



void sendMessageLoop(int clientSocket,char *username)
{
    fd_set readFds;
    FD_ZERO(&readFds);
    FD_SET(clientSocket, &readFds);
    FD_SET(STDIN_FILENO, &readFds);

    while (true)
    {
        fd_set tmpReadFds = readFds;
        int maxFd = max(clientSocket, STDIN_FILENO);

        // Use select to monitor file descriptors for readiness
        int readySock = select(maxFd + 1, &tmpReadFds, NULL, NULL, NULL);

        if (readySock == -1)
        {
            cerr << "[SYSTEM] Error in select\n";
            break;
        }
        else if (FD_ISSET(clientSocket, &tmpReadFds))
        {
            // Data available from the server
            size_t messageLen;
            int lenBytesRead = recv(clientSocket, &messageLen, sizeof(messageLen), 0);

            if (lenBytesRead <= 0)
            {
                cerr << "[SYSTEM] Server disconnected.\n";
                break;
            }
            else
            {
                
                int bytesRead = recv(clientSocket, buffer, messageLen, 0);

                if (bytesRead <= 0)
                {
                    cerr << "[SYSTEM] Server disconnected.\n";
                    break;
                }
                else
                {
                    buffer[bytesRead] = '\0';
                    pid_t pid = getpid();
                    kill(pid,SIGUSR1);
                }
            }
        }
        else if (FD_ISSET(STDIN_FILENO, &tmpReadFds))
        {
            // User input available
            char buff[MAX_MSG_LENGTH];
            cin.getline(buff, MAX_MSG_LENGTH);
            if(sendMessage(clientSocket,buff,username,NORMAL_MSG)==-1) break;
        }
    }
}

void msgRecvHdlr(int signum){
    cout << "[SYSTEM] Received broadcast from server: " << buffer << endl;
}

int main(int argv, char **argc)
{
    signal(SIGUSR1,msgRecvHdlr);
    const char *servIP = "127.0.0.1";
    

    // Initialize socket and connect to the server
    int clientSocket = initializeSocket(servIP,argc[1]);

    if (clientSocket != -1)
    {
        // Enter the message sending loop
        sendMessageLoop(clientSocket,argc[1]);

        // Close the socket when the loop ends
        close(clientSocket);
    }

    return 0;
}
