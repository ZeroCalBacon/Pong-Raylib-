// Pong.cpp
#include "raylib.h"

#include "Intro.h"
#include "Menu.h"
#include "GameStyles.h"

//Main driver code: puts title, menu, and gameplay together

enum class MenuState {
    INTRO,
    MENU,
    GAMESTYLE
};

int main() {
    //window dimensions
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
    //window 
    InitWindow(screenWidth, screenHeight, "Pong");

    Intro intro;
    Menu menu;
    GameStyles gameStyles;
    MenuState currentState = MenuState::INTRO;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {
            //intro section, credit | title
            case MenuState::INTRO:
                intro.Update();
                intro.Draw();
                //transition on mouse click
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = MenuState::MENU;
                }
                break;

            //menu section
            case MenuState::MENU:
                menu.Update();
                menu.Draw();
                break;
            
            default:
                break;
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
