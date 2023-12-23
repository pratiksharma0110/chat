#include "Login.hpp"

SDL_Rect chatNowBtn;

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

    SDL_Rect inputBox;
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
        if(EventHandler::OnClickListener::clicked(chatNowBtn)){
            std::cout << "chat now clicked\n";
        }
    }
}

void Login::loginScreen(){

    loginUI();

    eventLoop();

}