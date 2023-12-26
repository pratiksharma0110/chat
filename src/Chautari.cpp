#include "Chautari.hpp"

#include "clienthandler.hpp"

#include <thread>

SDL_Rect messageBox, left, right, sendBtn;

fd_set readFds;
int clientSocket;

char recvbuffer[1024];

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

void networkThread(int socketDescriptor, bool& quit) {
    while (!quit) {
        // Receive data from the socket
        // ssize_t bytesReceived = recv(socketDescriptor, recvbuffer, sizeof(recvbuffer) - 1, 0);
        // if (bytesReceived <= 0) {
        //     // Handle disconnection or error
        //     std::cout << "Connection closed or error occurred.\n";
        //     quit = true;
        //     break;
        // }
        size_t messageLen;
    int lenBytesRead = recv(clientSocket, &messageLen, sizeof(messageLen), 0);

    if (lenBytesRead <= 0)
    {
        std::cerr << "[SYSTEM] Server disconnected.\n";

    }
    else
    {

        int bytesRead = recv(clientSocket, recvbuffer, messageLen, 0);

        if (bytesRead <= 0)
        {
            std::cerr << "[SYSTEM] Server disconnected.\n";
        }
        else
        {
            recvbuffer[bytesRead] = '\0';
            // pid_t pid = getpid();
            // kill(pid, SIGUSR1);
        }
    }

 // Null-terminate the received data
        std::cout << "Received: " << recvbuffer << std::endl;
    }
}

void Chautari::eventLoop(){
    while(!EventHandler::close){
        chautariUI();
        if(recvbuffer[0] != '\0'){
            std::string bfr(recvbuffer);
            Message m;
            m.p = {"doge", bfr};
            m.r.y = 150 - 50 * messages.size();
            messages.push_back(m);
            recvbuffer[0] = '\0';
        }
        DisplayMessages();
        EventHandler::listen();
        if(EventHandler::OnClickListener::clicked(messageBox)){
            std::cout << "Input mode started\n";
            EventHandler::KeyEventListener::inputMode = true;
        }
        if(EventHandler::OnClickListener::clicked(sendBtn)){
            break;
        }
        if(sendMode){
            sendMessage(clientSocket, const_cast<char*>(EventHandler::KeyEventListener::inputBuffer.data()));
            EventHandler::KeyEventListener::inputBuffer = " ";
            EventHandler::KeyEventListener::inputMode = false;
            sendMode = false;
        }


        wm->SetText(messageBox, (EventHandler::KeyEventListener::inputBuffer.c_str()), 5, 5, {255,255,255,255}, 13);
        SDL_RenderPresent(wm->renderer);
        

        EventHandler::OnClickListener::c = {-1,-1};
    }
    
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


    // std::cout << "rcvd from login: " << username << "\n";

    // std::cout << "cast test: " << const_cast<char*>(username.data());

    // std::thread networkThread(networkThread, clientSocket, std::ref(EventHandler::close));
    std::thread nT(networkThread, clientSocket, std::ref(EventHandler::close));

    if(join(const_cast<char*>(username.data())) != -1){
        FD_ZERO(&readFds);
        FD_SET(clientSocket, &readFds);
        FD_SET(STDIN_FILENO, &readFds);
    }

    eventLoop();

    nT.join();

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