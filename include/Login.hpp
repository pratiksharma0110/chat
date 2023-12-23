#pragma once
#include "UI.hpp"

class Login{
    public:
    GUI::WindowManager* wm;
    Login(GUI::WindowManager* wm);
    void loginScreen();
    void eventLoop();
    void loginUI();
};