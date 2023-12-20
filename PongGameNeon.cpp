// PongGameNeon.cpp
#include "PongGameNeon.h"
#include "MathFunction.h"
#include <math.h>

//inherits from PongGame, this is "Neon" game style
//changes background to black and blue gradient
//changes objs to pink
//changes ball to gradient white and pink (glow eff.)

PongGameNeon::PongGameNeon() : PongGame() {
    screenWidth = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());

    paddleSpeed = 10.0f;
    ballSpeed = { 8.0f, 8.0f };

    gameState = COUNTDOWN;
    InitializeGame();

}

PongGameNeon::~PongGameNeon() {
}

void PongGameNeon::Update() {
    switch (gameState) {
        case COUNTDOWN:
            UpdateCountdown();
            break;
        case GAMEPLAY:
            UpdateGameplay();
            break;
        case RESET_GAMEPLAY:
            UpdateResetGameplay();
            break;
        case WINNER_GAMEOVER:
            UpdateWinnerGameover();
            break;
        case TRY_AGAIN:
            UpdateTryAgain();
            break;
        default:
            break;
    }

    elapsedTime += GetFrameTime();

    if (gameState == GAMEPLAY) {
        MovePlayerPaddle();
        MoveBotPaddle();
        MoveBall();
        CheckBallCollision();
        CheckGoal();
    }

    if (elapsedTime - lastSpeedIncreaseTime >= 10.0f) {
    float magnitude = sqrt(ballSpeed.x * ballSpeed.x + ballSpeed.y * ballSpeed.y);
    
    if (magnitude > 0.0f) {
            float normalizedX = ballSpeed.x / magnitude;
            float normalizedY = ballSpeed.y / magnitude;

            ballSpeed.x = normalizedX * 7.0f;
            ballSpeed.y = normalizedY * 7.0f;
        }

        lastSpeedIncreaseTime = elapsedTime;
    }
}

//changes here
void PongGameNeon::Draw() {
    BeginDrawing();

    ClearBackground(BLACK);

    //draw gradient background
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight() * 2, BLACK, BLUE);

    if (gameState != WINNER_GAMEOVER && gameState != TRY_AGAIN) {
        if (gameState != COUNTDOWN) {
            DrawText(TextFormat("%d", playerScore), screenWidth / 4, 20, 40, PINK);
            DrawText(TextFormat("%d", botScore), 3 * screenWidth / 4, 20, 40, PINK);

            for (int i = 0; i < GetScreenHeight(); i += 20) {
                DrawRectangle(screenWidth / 2, i, 4, 10, PINK);
            }

            DrawRectangleRec(playerPaddle, PINK);
            DrawRectangleRec(botPaddle, PINK);

            Vector2 ballCenter = { ball.x + ballRadius, ball.y + ballRadius };
            DrawCircleGradient(ballCenter.x, ballCenter.y, ballRadius, WHITE, PINK);
        }
    }

    if (gameState == COUNTDOWN) {
        DrawText(TextFormat("%d", countdownTimer / 60 + 1), screenWidth / 2 - 20, screenHeight / 2 - 20, 40, PINK);
    }

    if (gameState == WINNER_GAMEOVER) {
        int gameOverFontSize = 60;
        int winnerFontSize = 50;

        Vector2 gameOverTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Game Over", gameOverFontSize) / 2, static_cast<float>(screenHeight) / 2 - 50 };
        Vector2 winnerTextPosition;

        if (playerScore >= 5) {
            winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Player Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
        } else {
            winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Bot Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
        }

        DrawText("Game Over", gameOverTextPosition.x, gameOverTextPosition.y, gameOverFontSize, PINK);

        if (playerScore >= 5) {
            DrawText("Player Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, PINK);
        } else {
            DrawText("Bot Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, PINK);
        }

        static int tryAgainTimer = 180;

        tryAgainTimer--;

        if (tryAgainTimer <= 0) {
            gameState = TRY_AGAIN;
        }
    }

    if (gameState == TRY_AGAIN) {
        int tryAgainFontSize = 50;
        int optionFontSize = 30;

        Vector2 tryAgainTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Try Again?", tryAgainFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
        Vector2 yesOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Yes", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 50.0f };
        Vector2 noOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("No", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 100.0f };


        DrawText("Try Again?", tryAgainTextPosition.x, tryAgainTextPosition.y, tryAgainFontSize, PINK);

        Color optionColor = PINK;

        if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(MeasureText("Yes", optionFontSize)), static_cast<float>(optionFontSize) })) {
            optionColor = WHITE;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                playerScore = 0;
                botScore = 0;
                gameState = COUNTDOWN;
                InitializeGame();
            }
        }

        DrawText("Yes", yesOptionPosition.x, yesOptionPosition.y, optionFontSize, optionColor);

        optionColor = PINK;

        if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ noOptionPosition.x, noOptionPosition.y, static_cast<float>(MeasureText("No", optionFontSize)), static_cast<float>(optionFontSize) })) {
            optionColor = WHITE;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CloseWindow();
            }
        }

        DrawText("No", noOptionPosition.x, noOptionPosition.y, optionFontSize, optionColor);
    }

    EndDrawing();
}

void PongGameNeon::InitializeGame() {
    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);
}



void PongGameNeon::ResetPositions() {
    playerPaddle = { 20.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    botPaddle = { screenWidth - 40.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    ball = { screenWidth / 2 - 10.0f, screenHeight / 2 - 10.0f, 20.0f, 20.0f };

    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

    lastSpeedIncreaseTime = elapsedTime;
}


void PongGameNeon::UpdateCountdown() {
    countdownTimer--;

    if (countdownTimer > 0) {
        DrawText(TextFormat("%d", countdownTimer / 60 + 1), screenWidth / 2 - 20, screenHeight / 2 - 20, 40, SKYBLUE);
    } else {
        gameState = GAMEPLAY;
        InitializeGame();
    }
}

void PongGameNeon::UpdateTryAgain() {
    int tryAgainFontSize = 50;
    int optionFontSize = 30;

    Vector2 tryAgainTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Try Again?", tryAgainFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 yesOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Yes", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 50.0f };
    Vector2 noOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("No", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 100.0f };

    DrawText("Try Again?", tryAgainTextPosition.x, tryAgainTextPosition.y, tryAgainFontSize, PINK);

    Color optionColor = PINK;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(MeasureText("Yes", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = WHITE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            playerScore = 0;
            botScore = 0;
            gameState = COUNTDOWN;
            InitializeGame();
        }
    }

    DrawText("Yes", yesOptionPosition.x, yesOptionPosition.y, optionFontSize, optionColor);

    optionColor = PINK;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ noOptionPosition.x, noOptionPosition.y, static_cast<float>(MeasureText("No", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = WHITE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SetReturnToMenu();
        }
    }

    DrawText("No", noOptionPosition.x, noOptionPosition.y, optionFontSize, optionColor);

    return;
}

void PongGameNeon::UpdateWinnerGameover() {
    int gameOverFontSize = 60;
    int winnerFontSize = 50;

    Vector2 gameOverTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Game Over", gameOverFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 winnerTextPosition;

    if (playerScore >= 5) {
        winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Player Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
    } else {
        winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Bot Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
    }

    DrawText("Game Over", gameOverTextPosition.x, gameOverTextPosition.y, gameOverFontSize, PINK);

    if (playerScore >= 5) {
        DrawText("Player Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, PINK);
    } else {
        DrawText("Bot Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, PINK);
    }

    static int tryAgainTimer = 180;

    tryAgainTimer--;

    if (tryAgainTimer <= 0) {
        gameState = TRY_AGAIN;
    }

    return;
}

void PongGameNeon::CheckGoal() {
    if (ball.x <= 0) {
        botScore++;

        ResetPositions();
        gameState = RESET_GAMEPLAY;

        int randomDirectionX = GetRandomValue(0, 1);
        int randomDirectionY = GetRandomValue(0, 1);

        ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
        ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

        ballSpeed = { 8.0f, 8.0f };
    }

    if (ball.x + ballRadius * 2 >= GetScreenWidth()) {
        playerScore++;

        ResetPositions();
        gameState = RESET_GAMEPLAY;
    }

    if (playerScore >= 5 || botScore >= 5) {
        gameState = WINNER_GAMEOVER;
    }

    if (gameState == RESET_GAMEPLAY) {
        countdownTimer = 120;
    }
}

void PongGameNeon::UpdateResetGameplay() {
    countdownTimer--;

    if (countdownTimer <= 0) {
        ResetPositions();
        gameState = GAMEPLAY;

        InitializeGame();

        int randomDirectionX = GetRandomValue(0, 1);
        int randomDirectionY = GetRandomValue(0, 1);

        ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
        ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

        
    }
}

void PongGameNeon::MoveBotPaddle() {
    const float aiSpeed = 9.0f;

    float targetY = ball.y + ballRadius - botPaddle.height / 2;

    if (GetRandomValue(0, 100) < 90) {
        botPaddle.y = Lerp(botPaddle.y, targetY, aiSpeed * GetFrameTime());
    } else {
        float randomSpeed = GetRandomValue(-1, 1);
        botPaddle.y += randomSpeed;
    }

    if (botPaddle.y < 0) {
        botPaddle.y = 0;
    } else if (botPaddle.y + botPaddle.height > screenHeight) {
        botPaddle.y = screenHeight - botPaddle.height;
    }
}

void PongGameNeon::MoveBall() {
    ball.x += ballSpeed.x;
    ball.y += ballSpeed.y;

    if (ball.y <= 0 || ball.y + ball.height >= GetScreenHeight()) {
        ballSpeed.y *= -1;
    }
}

void PongGameNeon::CheckBallCollision() {
    Vector2 ballCenter = { ball.x + ballRadius, ball.y + ballRadius };

    if (CheckCollisionPointRec(ballCenter, playerPaddle)) {
        ballSpeed.x = fabs(ballSpeed.x);

        float relativeIntersectY = (playerPaddle.y + playerPaddle.height / 2) - ballCenter.y;
        float normalizedRelativeIntersectionY = (relativeIntersectY / (playerPaddle.height / 2));

        ballSpeed.y = normalizedRelativeIntersectionY * fabs(ballSpeed.x);

        if (IsKeyDown(KEY_W)) {
            ballSpeed.y -= 2.0f;
        }
        else if (IsKeyDown(KEY_S)) {
            ballSpeed.y += 2.0f;
        }

        if (ballSpeed.x < 0) {
            ballSpeed.x *= -2;
        }
    }

    if (CheckCollisionPointRec(ballCenter, botPaddle)) {
        ballSpeed.x = -fabs(ballSpeed.x);

        float relativeIntersectY = (botPaddle.y + botPaddle.height / 2) - ballCenter.y;
        float normalizedRelativeIntersectionY = (relativeIntersectY / (botPaddle.height / 2));

        ballSpeed.y = normalizedRelativeIntersectionY * fabs(ballSpeed.x);

        if (IsKeyDown(KEY_UP)) {
            ballSpeed.y -= 2.0f;
        }
        else if (IsKeyDown(KEY_DOWN)) {
            ballSpeed.y += 2.0f;
        }

        if (ballSpeed.x > 0) {
            ballSpeed.x *= -2;
        }
    }
}