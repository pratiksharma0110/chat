#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

using namespace std;


constexpr int MAX_CLIENTS = 10;
constexpr int MAX_USERNAME = 20;

typedef struct {
    int socket;
    char username[MAX_USERNAME]={0};
} User;


//Function to receive client messages
void recvMsg(int client_socket,User *clients,int i){
    // First, receive the message length
    size_t messageLen;
    int lenBytesRead = recv(client_socket, &messageLen, sizeof(messageLen), 0);

    if (lenBytesRead <= 0)
    {
        // Handle disconnect or error
        close(client_socket);
        clients[i].socket = 0;
        printf("[SYSTEM] Socket %d disconnected.\n", client_socket);
    }
    else
    {
        // Now, receive the actual message based on the received length
        char buffer[1024];
        int bytesRead = recv(client_socket, buffer, messageLen, 0);

        if (bytesRead <= 0)
        {
            // Handle disconnect or error
            close(client_socket);
            clients[i].socket = 0;
            printf("[SYSTEM] Socket %d disconnected.\n", client_socket);
        }
        else
        {
            // Process received data
            buffer[bytesRead] = '\0'; // Null-terminate the received data
            cout << "[SYSTEM] Received data from socket " << client_socket << ": " << buffer << endl;

            for (int j = 0; j < MAX_CLIENTS; ++j)
            {
                int dest_socket = clients[j].socket;
                if (dest_socket > 0)
                {
                    // Send the message to the destination client
                    send(dest_socket, &messageLen, sizeof(messageLen), 0);
                    send(dest_socket, buffer, messageLen, 0);
                }
            }
        }
    }
}

// Function to create a socket
int createSocket()
{
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket == -1)
    {
        cerr << "[SYSTEM] Error creating socket\n";
    }
    else
    {
        cout << "[SYSTEM] Socket Created Successfully\n";
    }
    return newSocket;
}

// Function to bind the created socket to a specific IP address and port
void bindSocket(int socket, const char *servIP, int port)
{
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(servIP);
    server_address.sin_port = htons(port);

    if (bind(socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "[SYSTEM] Error binding socket\n";
        close(socket);
        exit(-1);
    }
    else
    {
        cout << "[SYSTEM] Successfully binded to local port\n";
    }
}

// Function to set the socket to listen for incoming connections
void startListening(int socket)
{
    if (listen(socket, SOMAXCONN) < 0)
    {
        cerr << "[SYSTEM] Failed to start\n";
        exit(-1);
    }
    else
    {
        cout << "[SYSTEM] Started listening to local port\n";
    }
}

// Function to accept a connection from a client
User acceptConnection(int listenSocket,User *clients)
{
    sockaddr_in clientAddress;
    socklen_t clientAddrSize = sizeof(clientAddress);
    User client;
    client.socket = accept(listenSocket, (struct sockaddr *)&clientAddress, &clientAddrSize);

    if (client.socket > 0)
    {
        //read(client.socket,client.username,20);
        size_t messageLen;
        int lenBytesRead = recv(client.socket, &messageLen, sizeof(messageLen), 0);

        if (lenBytesRead <= 0)
        {
            // Handle disconnect or error
            close(client.socket);
            clients[client.socket-4].socket = 0;
            printf("[SYSTEM] Socket %d disconnected.\n", client.socket);
        }
        else
        {
            // Now, receive the actual message based on the received length
            char buffer[1024];
            int bytesRead = recv(client.socket, buffer, messageLen, 0);

            if (bytesRead <= 0)
            {
                // Handle disconnect or error
                close(client.socket);
                clients[client.socket-4].socket = 0;
                printf("[SYSTEM] Socket %d disconnected.\n", client.socket);
            }
            else
            {
                // Process received data
                buffer[bytesRead] = '\0'; // Null-terminate the received data
                strcpy(client.username,buffer);
                for (int j = 0; j < MAX_CLIENTS; ++j)
                {
                    int dest_socket = clients[j].socket;
                    if (dest_socket > 0)
                    {
                        // Send the message to the destination client
                        send(dest_socket, &messageLen, sizeof(messageLen), 0);
                        send(dest_socket, buffer, messageLen, 0);
                    }

                     if(clients[j].username){
                        // char msg[10 + MAX_USERNAME] = "UNAME:";
                        // strcat(msg, clients[j].username);
                        size_t len = strlen(clients[j].username);
                        send(client.socket, &len, sizeof(len), 0);
                        send(client.socket, clients[j].username, len, 0);
                    }
                }
            }
        }
        cout << "[SYSTEM] Accepted connection from "
             << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << "Username: " <<client.username<<endl;

        //write()
    }
    else
    {
        cerr << "[SYSTEM] Error accepting connection\n";
    }
    return client;
}

// Function to find the maximum socket descriptor among the listening socket and connected client sockets
int findMaxFd(int listenSocket, User *clients, fd_set &readFds)
{
    int maxFd = listenSocket;

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        int client_socket = clients[i].socket;
        if (client_socket > 0)
        {
            // Add each connected client socket to the read set
            FD_SET(client_socket, &readFds);

            // Update the maximum socket descriptor if needed
            if (client_socket > maxFd)
            {
                maxFd = client_socket;
            }
        }
    }

    return maxFd;
}

// Function to handle client sockets
void handleClientSockets(int listenSocket,  User *clients, fd_set &readFds)
{
    // Find the maximum socket descriptor
    int maxFd = findMaxFd(listenSocket, clients, readFds);

    // Use select to monitor file descriptors for readiness
    int readySock = select(maxFd + 1, &readFds, NULL, NULL, NULL);

    if (readySock == -1)
    {
        cerr << "[SYSTEM] Error\n";
    }
    else
    {
        // Check if the listening socket is ready for a new connection
        if (FD_ISSET(listenSocket, &readFds))
        {

            User client = acceptConnection(listenSocket,clients);

            for (int i = 0; i < MAX_CLIENTS; ++i)
            {
                if (clients[i].socket == 0)
                {
                    clients[i].socket = client.socket;
                    strcpy(clients[i].username, client.username);
                    break;
                }
            }
        }

        // Check each client socket for readiness
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            int client_socket = clients[i].socket;
            if (client_socket > 0 && FD_ISSET(client_socket, &readFds))
            {
                recvMsg(client_socket,clients,i);
            }
        }
    }
}


int main()
{
    fd_set masterFds, readFds;        // File descriptor sets for monitoring sockets
    const char *servIP = "127.0.0.1"; // Server IP address
    FD_ZERO(&masterFds);

    // Create a listening socket and add it to the master set
    int listenSocket = createSocket();
    FD_SET(listenSocket, &masterFds);

    // Bind the socket to a specific IP address and port, then start listening
    bindSocket(listenSocket, servIP, 4444);
    startListening(listenSocket);

    // Array to store client socket descriptors
    User client_sockets[MAX_CLIENTS];
    memset(client_sockets, 0, sizeof(client_sockets));

    while (true)
    {
        readFds = masterFds;

        handleClientSockets(listenSocket, client_sockets, readFds);
    }

    // Close the listening socket and all client sockets on exit
    close(listenSocket);
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (client_sockets[i].socket > 0)
        {
            close(client_sockets[i].socket);
        }
    }

    return 0;
}
