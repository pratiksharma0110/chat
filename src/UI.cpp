#include "UI.hpp"

namespace EventHandler {
    bool close = false;
    SDL_Event e;
    coord OnClickListener::c = {-1,-1};
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
    // SDL_RenderPresent(renderer);
}

void GUI::WindowManager::SetText(SDL_Rect parent, const char* text, int x, int y, SDL_Color c, int fontSize){
    TTF_Font* font = TTF_OpenFont(FONT, fontSize);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
    SDL_Color bg = {0,0,0,0};
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

    // SDL_Rect destRect = {parent.x + (parent.w - textSurface->w) / 2, parent.y + (parent.h - textSurface->h) / 2, textSurface->w, textSurface->h};
    SDL_Rect destRect = {parent.x + x, parent.y + y, textSurface->w, textSurface->h};

    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
    // SDL_RenderPresent(renderer);

}

bool EventHandler::OnClickListener::clicked(SDL_Rect r){
    bool ret = false;
    if(c.x != -1 && c.y != -1){
        ret = (c.x>=r.x && c.x <= r.x+r.w) && (c.y >=r.y && c.y <=r.y+r.h);
        c = {-1,-1};
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
        }
    }
}
