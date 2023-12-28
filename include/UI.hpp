#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>

#define WIDTH 1280
#define HEIGHT 768

#define TITLE "Digital Chautari"

#define FONT "/home/protikbruhh/Desktop/chat/static/whitrabt.ttf"

#define FONTSIZE_1 16
    
typedef struct{
    int r,g,b,a;
} color;

typedef struct{
    int x, y;
} coord;

extern bool sendMode;

namespace GUI{

    class WindowManager{
        public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        void CreateWindow();
        void Draw(color c, SDL_Rect r, bool fill);
        void Clear();
        void SetText(SDL_Rect parent, const char* text, int x, int y, SDL_Color c, int fontSize);
        static void FillInput(SDL_Rect inputBox, const char* text, int x, int y, SDL_Color c, int fontSize);
    };

    class Utils{
        public:
        void cursorBlink(){

        }

        static const char* getchrptr(std::string s){
            char * c = (char*)malloc(s.length());
            memset(c, 0, s.length());
            for(int i = 0; i < s.length(); i++){
                c[i] = s[i];
            }
            const char* cc = c;

            return cc;
        }
    };
}

namespace EventHandler{


    extern bool close;

    extern SDL_Event e;

    extern bool typing;
   
    void listen();

    class KeyEventListener{
        public:
        static bool inputMode;
        static std::string inputBuffer;
        // static SDL_Rect inputBox;
    };

    class OnClickListener{
        public:
        static coord c;
        static bool clicked(SDL_Rect r);

    };


}

