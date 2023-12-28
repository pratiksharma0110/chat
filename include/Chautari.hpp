#include "UI.hpp"

#include <vector>

#define MAX_MESSAGES 7

// Holds name and message of a sender (A user is also a sender to themselves)
class Profile{
    public:
    std::string name, message;
};

struct Message{
    Profile p;
    SDL_Rect r;
};

extern std::vector<std::string> connectedUsers; // A vector of string that contains the usernames of connected users
extern std::vector<Message> messages;   // A vector of messages sent on chautari

class Chautari{
    public:
    std::string username;   // Username of logged in user
    GUI::WindowManager* wm;

    Chautari(GUI::WindowManager* wm, std::string username);
    void chautari();
    void eventLoop();
    void chautariUI();
    void DisplayMessages();
    bool messageRecieved();
};