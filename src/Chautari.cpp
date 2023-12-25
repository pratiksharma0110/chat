#include "Chautari.hpp"


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>
#include <signal.h>

#define servPORT 8080

const char *servIP = "127.0.0.1";
int clientSocket;

SDL_Rect messageBox, left, right, sendBtn;

Chautari::Chautari(GUI::WindowManager* wm, std::string username){
    this->wm  = wm;
    this->username = username;
}

void Chautari::chautariUI(){
    wm->Clear();

    left = {1,1,WIDTH/2-310, HEIGHT-80};
    wm->Draw({255,255,255,255}, left, false);


    right = {WIDTH-(WIDTH/2-310),1,WIDTH/2-310, HEIGHT-80};
    wm->Draw({255,255,255,255}, right, false);

    messageBox = {left.x+left.w,HEIGHT-150,WIDTH-2*left.w};
    messageBox.h = 1+left.h-messageBox.y;
    wm->Draw({255,255,255,255}, messageBox, false);

    wm->SetText(left, "Welcome to", 20, 20, {255,255,255,255},30);

    wm->SetText(left, "Digital Chautari", 20, 60, {255,255,255,255},30);


    int c = 0;
    for(auto it=connectedUsers.begin(); it!=connectedUsers.end(); it++){
        wm->SetText(right, it->c_str(), 10, 10+30*c, {255,255,255,255}, 16);
        c++;
    }

}

void Chautari::eventLoop(){
    while(!EventHandler::close){
        chautariUI();
        DisplayMessages();
        EventHandler::listen();
        if(EventHandler::OnClickListener::clicked(messageBox)){
            std::cout << "Input mode started\n";
            EventHandler::KeyEventListener::inputMode = true;
        }
        if(EventHandler::OnClickListener::clicked(sendBtn)){
            EventHandler::KeyEventListener::inputBuffer = " ";
            EventHandler::KeyEventListener::inputMode = false;
            break;
        }


        wm->SetText(messageBox, (EventHandler::KeyEventListener::inputBuffer.c_str()), 5, 5, {255,255,255,255}, 13);
        SDL_RenderPresent(wm->renderer);
        

        EventHandler::OnClickListener::c = {-1,-1};
    }
    
}

int initializeSocket(const char *servIP,char *username)
{
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        std::cerr << "[SYSTEM] Error creating socket\n";
        return -1;
    }
    else
    {
        std::cout << "[SYSTEM] Socket Created Successfully\n";
    }

    // Set up the address family
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(servPORT);
    serverAddress.sin_addr.s_addr = inet_addr(servIP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "[SYSTEM] Error connecting to the server\n";
        close(clientSocket);
        return -1;
    }
    else
    {
        write(clientSocket,username,20);   
        std::cout << "[SYSTEM] Connected to the server at " << servIP << ":" << ntohs(serverAddress.sin_port) << "\n";
    }

    return clientSocket;
}


void Chautari::chautari(){

    // Message m1;
    // m1.p = {"owl", "hlo bros k xa"};
    // m1.r.y = 150 - 50 * messages.size();
    // messages.push_back(m1);

    // Message m2;
    // m2.p = {"doge", "im pedo"};
    // m2.r.y = 150 - 50 * messages.size();
    // messages.push_back(m2);

    // Message m3;
    // m3.p = {"chad", "chi k bhaneko esto dost"};
    // m3.r.y = 150 - 50 * messages.size();
    // messages.push_back(m3);

    // connectedUsers.push_back("doge");
    // connectedUsers.push_back("chad");
    // connectedUsers.push_back("owl");


    std::cout << "rcvd from login: " << username << "\n";

    std::cout << "cast test: " << const_cast<char*>(username.data());
    clientSocket = initializeSocket(servIP,const_cast<char*>(username.data()));

    eventLoop();

}

void Chautari::DisplayMessages(){
    if(!newSender.message.empty()){
        if(messages.size() >= MAX_MESSAGES){

            messages.erase(messages.begin());

        }

        Message m;
        m.p = newSender;
        m.r.y = 30 + 70 * messages.size();
        messages.push_back(m);
        newSender.name.clear();
        newSender.message.clear();
    }

    // wm->Clear();

    for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
        wm->SetText(messageBox, (it->p.name).c_str(), 70, -it->r.y-20, {255,255,255,255}, 16);
        wm->SetText(messageBox, (it->p.message).c_str(), 70, -it->r.y, {255,255,255,255}, 13);
    }

    // SDL_RenderPresent(wm->renderer);
}