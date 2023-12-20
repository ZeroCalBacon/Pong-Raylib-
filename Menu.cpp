// Menu.cpp

#include "Menu.h"

Menu::Menu() : selectedOption(0), currentGameStyle(0), currentGameInstance(nullptr) {}

Menu::~Menu() {
    delete currentGameInstance;
}

void Menu::Update() {
    Vector2 mousePoint = GetMousePosition();

    //check collision with each menu option and update selectedOption
    if (CheckCollisionPointRec(mousePoint, { static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Play", 30) / 2,
                                             static_cast<float>(GetScreenHeight()) / 2 - 60,  // Adjusted Y-coordinate for more spacing
                                             static_cast<float>(MeasureText("Play", 30)),
                                             30 }))
        selectedOption = 0;
    else if (CheckCollisionPointRec(mousePoint, { static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Styles", 30) / 2,
                                                  static_cast<float>(GetScreenHeight()) / 2,  // Adjusted Y-coordinate for more spacing
                                                  static_cast<float>(MeasureText("Styles", 30)),
                                                  30 }))
        selectedOption = 1;
    else if (CheckCollisionPointRec(mousePoint, { static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Exit", 30) / 2,
                                                  static_cast<float>(GetScreenHeight()) / 2 + 60,  // Adjusted Y-coordinate for more spacing
                                                  static_cast<float>(MeasureText("Exit", 30)),
                                                  30 }))
        selectedOption = 2;

    //gamestyle instance
    GameStyles gamestyle;
    
    //check if the user clicked then perform action
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (selectedOption) {
            //"Play" option, plays game based on game style (set to "Default" if not chosen)
            case 0:
                StartGame();
                break;

            //"Styles" option, different game styles to choose
            case 1:
                //transition to game style submenu
                while (!gamestyle.ShouldGoBackToMenu()) {
                    BeginDrawing();
                    ClearBackground(BLACK);

                    gamestyle.Update();
                    gamestyle.Draw();
                    
                    //update the current game style in the Menu class
                    currentGameStyle = gamestyle.GetCurrentGameStyle();

                    EndDrawing();
                }
                break;

            //"Exit" option, close window and program
            case 2:
                // Exit the program
                CloseWindow();

            default:
                break;
        }
    }
}


void Menu::Draw() {
    ClearBackground(BLACK);

    //draw options 
    DrawText("Play", static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Play", 30) / 2, static_cast<float>(GetScreenHeight()) / 2 - 60, 30, (selectedOption == 0) ? SKYBLUE : WHITE);
    DrawText("Styles", static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Styles", 30) / 2, static_cast<float>(GetScreenHeight()) / 2, 30, (selectedOption == 1) ? SKYBLUE : WHITE);
    DrawText("Exit", static_cast<float>(GetScreenWidth()) / 2 - MeasureText("Exit", 30) / 2, static_cast<float>(GetScreenHeight()) / 2 + 60, 30, (selectedOption == 2) ? SKYBLUE : WHITE);
}

//function for "Play" option, plays game based on game style
void Menu::StartGame() {
    //release the memory for the previous game style
    delete currentGameInstance;

    //creates an instance of the selected game style
    switch (currentGameStyle) {
        //default game style
        case 0:
            currentGameInstance = new PongGame();
            break;

        //inverted game style
        case 1:
            currentGameInstance = new PongGameInverted();
            break;

        //vault tec game style
        case 2:
            currentGameInstance = new PongGameVaultTec();
            break;

        //neon game style
        case 3:
            currentGameInstance = new PongGameNeon();
            break;

        //rainbow game style
        case 4:
            currentGameInstance = new PongGameRainbow();
            break;

        //tron game style
        case 5:
            currentGameInstance = new PongGameTron();
            break;

        //reversal game style
        case 6: 
            currentGameInstance = new PongGameReversal();
            break;

        default:
            break;
    }
    

    //play the game based on current game instance
    while (!WindowShouldClose()) {
        if (currentGameInstance->ShouldReturnToMenu()) {
            currentGameInstance->ResetGameState();
            ClearBackground(BLACK);
            break;
        }
        //calls corresponding Update() and Draw() functions to play
        currentGameInstance->Update();
        currentGameInstance->Draw();
    }
}

void Menu::HandleGameStyleSelection() {
    //update current game style based on the GameStyles submenu
    currentGameStyle = gamestyle.GetCurrentGameStyle();
}