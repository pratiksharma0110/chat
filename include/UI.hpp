#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>

#define WIDTH 800
#define HEIGHT 600

#define TITLE "Digital Chautari"

#define FONT "/home/boxhead/Documents/doge/project/static/tnr.ttf"

#define FONTSIZE_1 16
    
typedef struct{
    int r,g,b,a;
} color;

namespace GUI{

    class WindowManager{
        public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        void CreateWindow();
        void Draw(color c, SDL_Rect r, bool fill);
        void SetText(SDL_Rect parent, const char* text, int x, int y, SDL_Color c, int fontSize);
    };

    class Utils{
        public:
        void cursorBlink(){

        }
    };
}

namespace EventHandler{


    extern bool close;

    extern SDL_Event e;
   
    void listen();

    class KeyEventListener{

    };

    class OnClickListener{
        public:
        void onClick(SDL_Rect r);

    };

}