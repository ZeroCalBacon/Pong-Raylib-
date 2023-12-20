// GameStyles.cpp

#include "GameStyles.h"
#include "Menu.h"

GameStyles::GameStyles() : selectedOption(0), goBackToMenu(false), currentGameStyle(0) {}

GameStyles::~GameStyles() {}

void GameStyles::Update() {
    Vector2 mousePoint = GetMousePosition();

    //check collision with each submenu option and update
    const float yOffset = 30.0f;
    const float startX = static_cast<float>(GetScreenWidth()) / 2;
    const float fontSize = 30;

    const char* menuOptions[] = {"Default", "Inverted", "Vault-Tec", "Neon", "Rainbow", "Tron", "Reversal", "Menu"};

    for (int i = 0; i < 8; ++i) {
        float y = static_cast<float>(GetScreenHeight()) / 2 - 140 + i * (yOffset + 20);

        Rectangle textBounds = {startX - static_cast<float>(MeasureText(menuOptions[i], fontSize)) / 2, y, static_cast<float>(MeasureText(menuOptions[i], fontSize)), fontSize};

        if (CheckCollisionPointRec(mousePoint, textBounds)) {
            selectedOption = i;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (selectedOption) {
            //default game style
            case 0:
            //inverted game style
            case 1:
            //Vault Tec game style
            case 2:
            //neon game style
            case 3:
            //rainbow game style
            case 4:
            //tron game style
            case 5:
            //cases 0-5 fall through for green text (current game style selection)
            //reversal
            case 6:
                currentGameStyle = selectedOption;
                break;

            //stop here (green text)
            //back to menu
            case 7:
                goBackToMenu = true;
                break;

            default:
                break;
        }
    }
}

void GameStyles::Draw() {
    ClearBackground(BLACK);

    //draw submenu options vertically
    const float yOffset = 30.0f;
    const float startX = static_cast<float>(GetScreenWidth()) / 2;
    const float fontSize = 30;

    const char* menuOptions[] = {"Default", "Inverted", "Vault-Tec", "Neon", "Rainbow", "Tron", "Reversal", "Menu"};

    for (int i = 0;  i < 8; ++i) {
        float y = static_cast<float>(GetScreenHeight()) / 2 - 140 + i * (yOffset + 20);
        DrawText(menuOptions[i], startX - MeasureText(menuOptions[i], fontSize) / 2, y, fontSize, (selectedOption == i) ? SKYBLUE : WHITE);

        //highlight current game style in green
        if (i == currentGameStyle) {
            DrawText(menuOptions[i], startX - MeasureText(menuOptions[i], fontSize) / 2, y, fontSize, GREEN);
        }
    }
}

bool GameStyles::ShouldGoBackToMenu() const {
    return goBackToMenu;
}

int GameStyles::GetCurrentGameStyle() const {
    return currentGameStyle;
}
