#include "UI.hpp"

// Adding 4 different font sizes
TTF_Font* font13, *font16, *font20, *font30;

bool sendMode = false;  // Initializing sendMode to false (Purpose of this boolean is in the header)

// Initializing some variables
namespace EventHandler {
    bool close = false;
    SDL_Event e;
    coord OnClickListener::c = {-1,-1}; // Initializing c to -1,-1 to represent that no mouse click as occured (-1,-1 being outside of screen)
    bool KeyEventListener::inputMode = false;
    std::string KeyEventListener::inputBuffer = " ";
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
            case SDL_QUIT:  // e.type == SDL_QUIT when close window [X] button is pressed
            break;

            case SDL_MOUSEBUTTONDOWN:   // e.type == SDL_MOUSEBUTTONDOWN when mouse click is detected
            EventHandler::OnClickListener::c = {e.button.x, e.button.y};    // Storing the co-ordinates of mouse click in 'c'
            break;

            case SDL_KEYDOWN:   // When a keyboard button is pressed
            if(e.key.keysym.sym == SDLK_RETURN){ // if enter pressed
                sendMode = true;
                EventHandler::KeyEventListener::inputMode = false;
            }
            else{ // if the pressed button is something other than enter

                if(e.key.keysym.sym == SDLK_BACKSPACE && !EventHandler::KeyEventListener::inputBuffer.empty()){
                    EventHandler::KeyEventListener::inputBuffer.pop_back();   // if backspace pressed then remove the last character from input buffer
                }

            }
            
            break;

            case SDL_TEXTINPUT: // if a series of keys detected
            if(EventHandler::KeyEventListener::inputMode){ // if input mode is true then append the keystrokes into input buffer
                EventHandler::KeyEventListener::inputBuffer += e.text.text;
            }
            break;
        }
    }
}
