#include "UI.hpp"

class Chautari{
    public:
    std::string username;
    GUI::WindowManager* wm;
    Chautari(GUI::WindowManager* wm, std::string username);
    void chautari();
    void eventLoop();
    void chautariUI();
};