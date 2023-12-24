#include "Login.hpp"
#include "Chautari.hpp"

SDL_Rect chatNowBtn;
SDL_Rect inputBox;


Login::Login(GUI::WindowManager* wm){
    this->wm = wm;
}

void Login::loginUI(){
   
    // Login Box
    SDL_Rect r;
    r.h=250;
    r.w= 450;
    r.y = (HEIGHT-r.h)/2;
    r.x = (WIDTH-r.w)/2;
    color c = {255, 255, 255, 0};
    wm->Draw(c, r, false);

    wm->SetText(r, "Login to Chautari", (r.w-r.x)/2 - 50, -100, {255,255,255,255}, 30);
    wm->SetText(r, "Enter a username:",(r.w - 16)/2 - 40, (r.h-16)/2 - 70, {255,255,255,255}, 16);

//   wm->SetText(r, "test:",40, 40, {255,255,255,255}, 16);

    inputBox.h=30;
    inputBox.w= 200;
    inputBox.y = (HEIGHT-inputBox.h)/2 - 40;
    inputBox.x = (WIDTH-inputBox.w)/2;
    wm->Draw({255, 255, 255, 0}, inputBox, false);


    chatNowBtn.h=60;
    chatNowBtn.w= 100;
    chatNowBtn.y = (HEIGHT-chatNowBtn.h)/2 + 50;
    chatNowBtn.x = (WIDTH-chatNowBtn.w)/2;
    wm->Draw({255, 255, 255, 0}, chatNowBtn, false);
   
    wm->SetText(chatNowBtn, "Chat Now", chatNowBtn.w/2 - 38, chatNowBtn.h/2 -10, {255,255,255,255}, 16);



    SDL_RenderPresent(wm->renderer);

}


void Login::eventLoop(){
    while(!EventHandler::close){
        EventHandler::listen();
        if(EventHandler::OnClickListener::clicked(inputBox)){
            std::cout << "Input mode started\n";
            EventHandler::KeyEventListener::inputMode = true;
        }
        if(EventHandler::OnClickListener::clicked(chatNowBtn)){
            std::cout << "Your username: ";
            std::cout << EventHandler::KeyEventListener::inputBuffer << "\n";
            EventHandler::KeyEventListener::inputBuffer = " ";
            EventHandler::KeyEventListener::inputMode = false;
            break;
        }

        if(EventHandler::typing){
            wm->SetText(inputBox, GUI::Utils::getchrptr(EventHandler::KeyEventListener::inputBuffer), 5, 5, {255,255,255,255}, 13);
            EventHandler::typing = false;
            SDL_RenderPresent(wm->renderer);
        }

        // std::cout << GUI::Utils::stringToCharPtr(EventHandler::KeyEventListener::inputBuffer) << "\n";
        // wm->SetText(inputBox, GUI::Utils::getchrptr(EventHandler::KeyEventListener::inputBuffer), 5, 5, {255,255,255,255}, 13);
        // wm->SetText(inputBox, "testusername", 5, 5, {255,255,255,255}, 13);
        EventHandler::OnClickListener::c = {-1,-1};
    }
}

void Login::loginScreen(){

    loginUI();

    eventLoop();
    
    (new Chautari(wm, EventHandler::KeyEventListener::inputBuffer))->chautari();

}