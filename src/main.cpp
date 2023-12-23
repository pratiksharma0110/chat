#include "UI.hpp"
#include "Login.hpp"

bool isLoggedIn = false;

int main(){
    GUI::WindowManager wm;
    wm.CreateWindow();

    Login l(&wm);

    
    if(!isLoggedIn){
        l.loginScreen();
        isLoggedIn = true;
    }

}