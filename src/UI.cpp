#include "UI.hpp"


TTF_Font* font13, *font16, *font20, *font30;

bool sendMode = false;

namespace EventHandler {
    bool close = false;
    SDL_Event e;
    coord OnClickListener::c = {-1,-1};
    bool KeyEventListener::inputMode = false;
    std::string KeyEventListener::inputBuffer = " ";
    SDL_Rect inputBox = {0,0,0,0};
    bool typing = false;
}

void GUI::WindowManager::CreateWindow(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
		printf("Error: %s\n", SDL_GetError());
		exit(1);
	}
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
    }

    font13 = TTF_OpenFont(FONT, 13);    
    font16 = TTF_OpenFont(FONT, 16);    
    font20 = TTF_OpenFont(FONT, 20);
    font30 = TTF_OpenFont(FONT, 30);    

    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
}

void GUI::WindowManager::Draw(color c, SDL_Rect r, bool fill){
    SDL_SetRenderDrawColor(renderer,c.r, c.g, c.b, c.a);
    if(fill){
        SDL_RenderFillRect(renderer, &r);
    }
    SDL_RenderDrawRect(renderer, &r);
}


void GUI::WindowManager::SetText(SDL_Rect parent, const char* text, int x, int y, SDL_Color c, int fontSize){
    if(text[0] != '\0'){
        if (!font13 || !font16 || !font30) {
            std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
        }
        SDL_Color bg = {0,0,0,0};
        TTF_Font* font;
        switch(fontSize){
            case 13:
            font = font13;
            break;

            case 20:
            font = font20;
            break;

            case 16:
            font = font16;
            break;

            case 30:
            font=font30;
            break;
        }
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, (const char*)text, c);
        
        if (!textSurface) {
            std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
        }
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_FreeSurface(textSurface);
        SDL_Rect destRect = {parent.x + x, parent.y + y, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
    }
}

void GUI::WindowManager::Clear(){
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
}

bool EventHandler::OnClickListener::clicked(SDL_Rect r){
    bool ret = false;
     if(c.x != -1 && c.y != -1){
              ret = (c.x>=r.x && c.x <= r.x+r.w) && (c.y >=r.y && c.y <=r.y+r.h);
    }
    return ret;
}

void EventHandler::listen(){
    while(SDL_PollEvent(&EventHandler::e)){

        switch (EventHandler::e.type)
        {
            case SDL_QUIT:
            close = true;
            break;

            case SDL_MOUSEBUTTONDOWN:
            EventHandler::OnClickListener::c = {e.button.x, e.button.y};
            break;

            case SDL_KEYDOWN:
            if(e.key.keysym.sym == SDLK_RETURN){
                sendMode = true;
                EventHandler::KeyEventListener::inputMode = false;
            }
            else{

                if(e.key.keysym.sym == SDLK_BACKSPACE && !EventHandler::KeyEventListener::inputBuffer.empty()){
                    EventHandler::KeyEventListener::inputBuffer.pop_back();   
                }

            }
            
            break;

            case SDL_TEXTINPUT:
            if(EventHandler::KeyEventListener::inputMode){
                EventHandler::KeyEventListener::inputBuffer += e.text.text;
            }
            break;
        }
    }
}
