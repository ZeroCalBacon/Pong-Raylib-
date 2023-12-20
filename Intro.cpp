// Intro.cpp

#include "Intro.h"
#include "raylib.h"

Intro::Intro() : introTimer(0.0f), showTitle(false) {}

Intro::~Intro() {}

void Intro::Update() {
    //update the timer
    introTimer += GetFrameTime();

    //after 2 seconds, show the title
    if (introTimer >= 2.0f) {
        showTitle = true;
    }

    //check user input to transition to the menu
    if (showTitle && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showTitle = false;
    }
}

void Intro::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (introTimer < 2.0f) {
        DrawText("Made by Franz", GetScreenWidth() / 2 - MeasureText("Made by Franz", 40) / 2, GetScreenHeight() / 2 - 40, 40, WHITE);
    }
    else if (showTitle) {
        DrawText("Pong", GetScreenWidth() / 2 - MeasureText("Pong", 60) / 2, GetScreenHeight() / 2 - 60, 60, WHITE);
        DrawText("Click to Start", GetScreenWidth() / 2 - MeasureText("Click to Start", 20) / 2, GetScreenHeight() - 40, 20, WHITE);
    }
}
