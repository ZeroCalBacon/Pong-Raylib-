// GameStyles.h

#ifndef GAMESTYLES_H
#define GAMESTYLES_H

#include "raylib.h"

class GameStyles {
public:
    GameStyles();
    ~GameStyles();

    //handling submenu navigation
    void Update();
    //rendering submenu options
    void Draw();

    //if the user wants to go back to the menu
    bool ShouldGoBackToMenu() const;
    //grabs current game style
    int GetCurrentGameStyle() const;


private:
    int selectedOption;
    bool goBackToMenu;
    
    int currentGameStyle;
};

#endif // GAMESTYLES_H