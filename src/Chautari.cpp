#include "Chautari.hpp"

#include "clienthandler.hpp"

#include <thread>
#include <algorithm>

SDL_Rect messageBox, left, right, sendBtn;

fd_set readFds;
int clientSocket;

char recvbuffer[1024];

std::string recvUsername, recvMsgBfr;
std::string type;

std::vector<std::string> connectedUsers;

Chautari::Chautari(GUI::WindowManager* wm, std::string username){
    this->wm  = wm;
    this->username = username;
}

void splitter(){
    std::string rcvbfr(recvbuffer);
    size_t colonPosition = rcvbfr.find(':');
    if (colonPosition != std::string::npos) {
        std::string substring = rcvbfr.substr(0, colonPosition);
        std::string restOfString = rcvbfr.substr(colonPosition + 1);

        type = substring;

        if(type == "UNAME"){
                recvUsername = restOfString;
                recvMsgBfr = "";
        }
        else if(type == "MSG"){
             size_t colonPosition2 = restOfString.find(':');
             std::string substring2 = restOfString.substr(0, colonPosition2);
             recvUsername = substring2;
             recvMsgBfr = restOfString.substr(colonPosition2 + 1);
        }
    }

}

void updateUsers(std::vector<std::string>& list, const std::string& username) {
    if (!username.empty() && std::find(list.begin(), list.end(), username) == list.end()) {
        std::cout << "user online detected: " << username <<"\n";
        list.push_back(username);
    } 
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

void networkThread(int socketDescriptor) {
    while (!EventHandler::close) {
        EventHandler::listen();
        size_t messageLen;
    int lenBytesRead = recv(clientSocket, &messageLen, sizeof(messageLen), 0);

    if (lenBytesRead <= 0)
    {

        perror("error: ");

    }
    else
    {

        int bytesRead = recv(clientSocket, recvbuffer, messageLen, 0);

        if (bytesRead <= 0)
        {
            perror("error: ");
            // exit(2);
        }
        else
        {
            splitter();
            updateUsers(connectedUsers, recvUsername);
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
        // updateUsers(connectedUsers, recvUsername);
        if(recvbuffer[0] != '\0'){
            std::string bfr(recvbuffer);
            // splitter();
            if(type == "MSG"){
                Message m;
                m.p = {recvUsername, recvMsgBfr};
                // m.r.y = 150 - 50 * messages.size();
                if(messages.size()>0){
                    std::cout << messages.begin()->p.message << " = begin \n";
                    std::cout << (messages.end()-1)->p.message << " = end-1\n";
                    m.r.y = (messages.end()-1)->r.y;
                    std::cout << m.r.y <<"\n";
                    for(auto it = messages.begin(); it != messages.end(); it++){
                        it->r.y+=50;
                    }
                }
                else{
                    std::cout << "first msg\n";
                    m.r.y = 150;
                }
                messages.push_back(m);
                recvbuffer[0] = '\0';
            }
            else{
                // std::cout << recvUsername << " joined\n";
            }
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
            if(EventHandler::KeyEventListener::inputBuffer != " " && !EventHandler::KeyEventListener::inputBuffer.empty()){
                sendMessage(clientSocket, const_cast<char*>(EventHandler::KeyEventListener::inputBuffer.data()), const_cast<char*>(username.data()), NORMAL_MSG);
            }
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
    // std::thread nT(networkThread, clientSocket, std::ref(EventHandler::close));

    if(join(const_cast<char*>(username.data())) != -1){
        std::cout << "success!";
        FD_ZERO(&readFds);
        FD_SET(clientSocket, &readFds);
        FD_SET(STDIN_FILENO, &readFds);
    }
    else{
        std::cout <<"join bhayena\n";
    } 
    
    std::thread nT(networkThread, clientSocket);

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
        m.r.y = 30-70 * messages.size();
        messages.push_back(m);
        newSender.name.clear();
        newSender.message.clear();
    }

    // wm->Clear();

    for (auto it = messages.begin(); it != messages.end(); ++it) {
        wm->SetText(messageBox, (it->p.name).c_str(), 70, -it->r.y-20, {255,255,255,255}, 16);
        wm->SetText(messageBox, (it->p.message).c_str(), 70, -it->r.y, {255,255,255,255}, 13);
    }

    // SDL_RenderPresent(wm->renderer);
}