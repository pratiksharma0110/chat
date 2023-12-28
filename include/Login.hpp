#pragma once
#include "UI.hpp"

class Login{
    public:
    GUI::WindowManager* wm; // An object of window manager to help us with rendering the login UI
    Login(GUI::WindowManager* wm);
    void loginScreen();     // First function to be called in login page
    void eventLoop();       // The event loop
    void loginUI();         // Function where all the UI is designed
};