/*

This is part handles all the GUI components and the event handlers. 
Things like creation of a window, rendering shapes, texts, images, etc. are handled 
by the GUI part (GUI namespace) of this header.

Handling events like, mouse click events, keyboard events, etc. are handled
by the members of EventHandler namespace.


*/


#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>

#define WIDTH 1280
#define HEIGHT 768

#define TITLE "Digital Chautari"

#define FONT "/home/protikbruhh/Desktop/chat/static/whitrabt.ttf"
    
typedef struct{
    int r,g,b,a;
} color;

typedef struct{
    int x, y;
} coord;

extern bool sendMode; // True when "Enter" key is pressed. Must be set to False immediately after use.

namespace GUI{

    class WindowManager{
        public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        void CreateWindow();    // Call this function to create a window
        void Draw(color c, SDL_Rect r, bool fill); // Call this function to draw a rectangle with given dimensions
        void Clear(); // Call this to clear the window
        void SetText(SDL_Rect parent, const char* text, int x, int y, SDL_Color c, int fontSize); // Call this to render a text
    };

    class Utils{

        // This class will contain other utilities such as rendering a blinking cursor when clicked on an input box

    };

}

namespace EventHandler{


    extern bool close; // This gets set to "True" when the close window [X] button is pressed 
    // All the event loops use this boolean as their condition. When this is True, event loops break and the window closes.

    extern SDL_Event e; // Stores events such as mouse click and key inputs as an abstract data type

   
    void listen(); // Call this function inside an event loop to keep checking for any events

    // Event handler for keyboard events
    class KeyEventListener{
        public:
        static bool inputMode;  // This should be set to "True" when user clicks on any input box and "False" when Enter key or any available button on screen is pressed
        // When inputMode is True, whatever user types gets stored in the inputBuffer below. 
        static std::string inputBuffer;
    };

    // Event handler for mouse events
    class OnClickListener{
        public:
        static coord c; // When a user clicks anywhere on the screen with their mouse, the co-ordinates of the position of mouse is stored in this variable
        static bool clicked(SDL_Rect r); // Returns "True" if the co-ordinates of 'c' lies within the passed rectangle (SDL_Rect r)
    };


}

