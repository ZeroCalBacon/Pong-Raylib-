// Menu.h

#ifndef MENU_H
#define MENU_H

#include "raylib.h"

#include "GameStyles.h"
#include "PongGame.h"
#include "PongGameInverted.h"
#include "PongGameVaultTec.h"
#include "PongGameNeon.h"
#include "PongGameRainbow.h"
#include "PongGameTron.h"
#include "PongGameReversal.h"

class Menu {
public:
    Menu();
    ~Menu();

    //functions called
    //handling menu navigation
    void Update(); 
    //rendering menu options
    void Draw(); 

    bool drawGameBoard;
    

private:
    //store the selected menu option
    int selectedOption;
    //store the current game style index
    int currentGameStyle;

    //instances
    PongGame* currentGameInstance;
    GameStyles gamestyle;

    void StartGame();
    void HandleGameStyleSelection();
};

#endif // MENU_H