#include "Login.hpp"
#include "Chautari.hpp"

SDL_Rect chatNowBtn; // Chat now button
SDL_Rect inputBox;   // Input box for user input

std::string username;   // Buffer to store the entered username

Login::Login(GUI::WindowManager* wm){
    this->wm = wm;
}

void Login::loginUI(){

    wm->Clear();

    // Initializing an SDL_Rect with dimensions
    SDL_Rect r;
    r.h=250;
    r.w= 450;
    r.y = (HEIGHT-r.h)/2;
    r.x = (WIDTH-r.w)/2;
    color c = {255, 255, 255, 0};
    
    wm->Draw(c, r, false); // Drawing a rectangle with above dimensions and color


    // Displaying 2 texts at points relative to the above rectangle
    wm->SetText(r, "Welcome to Digital Chautari", (r.w-r.x)/2-5, -100, {255,255,255,255}, 30);
    wm->SetText(r, "Enter a username:",(r.w - 16)/2 -90, (r.h-16)/2 - 70, {255,255,255,255}, 20);

    // Setting dimensions of the input box
    inputBox.h=30;
    inputBox.w= 200;
    inputBox.y = (HEIGHT-inputBox.h)/2 - 40;
    inputBox.x = (WIDTH-inputBox.w)/2;
    wm->Draw({255, 255, 255, 0}, inputBox, false); // Drawing the input box


    // Setting dimensions of the chat now button
    chatNowBtn.h=60;
    chatNowBtn.w= 110;
    chatNowBtn.y = (HEIGHT-chatNowBtn.h)/2 + 50;
    chatNowBtn.x = (WIDTH-chatNowBtn.w)/2;
    wm->Draw({255, 255, 255, 0}, chatNowBtn, false); // Drawing the chat now button
   
    // Setting "Chat Now" text inside the chat now button rectangle
    wm->SetText(chatNowBtn, "Chat Now", chatNowBtn.w/2-45 , chatNowBtn.h/2 -10, {255,255,255,255}, 20);

}


void Login::eventLoop(){
    while(!EventHandler::close){
        loginUI();  // Rendering the loginUI on each loop 
        EventHandler::listen(); // Listening for any kind of events
        if(EventHandler::OnClickListener::clicked(inputBox)){   // if a mouse click event occured and the clicked co-ordinate lies within the input text box then setting inputmode to true
            std::cout << "Input mode started\n";
            EventHandler::KeyEventListener::inputMode = true;
        }
        if(EventHandler::OnClickListener::clicked(chatNowBtn)){ // if chatnow button clicked (same logic as above)
            std::cout << "Your username: ";
            std::cout << EventHandler::KeyEventListener::inputBuffer << "\n";
            username=EventHandler::KeyEventListener::inputBuffer;   // Setting username equal to the input buffer
            EventHandler::KeyEventListener::inputBuffer = " ";  // Clearing the input buffer
            EventHandler::KeyEventListener::inputMode = false;  // Setting input mode to false
            break;  // Breaking the event loop
        }

        //  Updating the input box with input buffer (whatever user typed)
        wm->SetText(inputBox, EventHandler::KeyEventListener::inputBuffer.c_str(), 5, 5, {255,255,255,255}, 13);

        // Updating the screen with the latest renderings
        SDL_RenderPresent(wm->renderer);

        // Resetting co-ordinates of mouse click
        EventHandler::OnClickListener::c = {-1,-1};
    }
}

void Login::loginScreen(){

    eventLoop();
    
    // Event loop breaks when chat now is pressed. 
    // So lets go to chautari with the same window manager and the given username
    (new Chautari(wm, username))->chautari();



}