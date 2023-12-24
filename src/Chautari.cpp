#include "Chautari.hpp"

Chautari::Chautari(GUI::WindowManager* wm, std::string username){
    this->wm  = wm;
    this->username = username;
}

void Chautari::chautariUI(){
    wm->Clear();

    SDL_Rect left = {1,1,WIDTH/2-310, HEIGHT-80};
    wm->Draw({255,255,255,255}, left, false);


    SDL_Rect right = {WIDTH-(WIDTH/2-310),1,WIDTH/2-310, HEIGHT-80};
    wm->Draw({255,255,255,255}, right, false);

    SDL_RenderPresent(wm->renderer);
}

void Chautari::eventLoop(){
    while(!EventHandler::close){
        EventHandler::listen();
    }
}

void Chautari::chautari(){

    chautariUI();

    eventLoop();

}