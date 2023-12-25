#include "Chautari.hpp"

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

void Chautari::chautari(){

    Message m1;
    m1.p = {"owl", "hlo bros k xa"};
    m1.r.y = 30 - 50 * messages.size();
    messages.push_back(m1);

    Message m2;
    m2.p = {"doge", "im pedo"};
    m2.r.y = 30 - 50 * messages.size();
    messages.push_back(m2);

    Message m3;
    m3.p = {"chad", "chi k bhaneko esto dost"};
    m3.r.y = 30 - 50 * messages.size();
    messages.push_back(m3);


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