#include "UI.hpp"

#include <vector>

#define MAX_MESSAGES 7

class Profile{
    public:
    std::string name, message;
};

struct Message{
    Profile p;
    SDL_Rect r;
};


class Chautari{
    public:
    std::string username;
    GUI::WindowManager* wm;

    std::vector<Message> messages;
    Profile newSender;

    std::vector<std::string> connectedUsers;

    

    Chautari(GUI::WindowManager* wm, std::string username);
    void chautari();
    void eventLoop();
    void chautariUI();
    void DisplayMessages();
    bool messageRecieved();
};