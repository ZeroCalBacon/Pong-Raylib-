// PongGameInverted.cpp
#include "PongGameInverted.h"
#include "MathFunction.h"
#include <math.h>

//inherits from PongGame, this is "Inverted" game style
//changes background and objs to white, literally inverts "Default"

PongGameInverted::PongGameInverted() : PongGame() {
    screenWidth = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());

    paddleSpeed = 10.0f;
    ballSpeed = { 8.0f, 8.0f };

    gameState = COUNTDOWN;
    InitializeGame();
    
}

PongGameInverted::~PongGameInverted() {
}

void PongGameInverted::Update() {
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

void PongGameInverted::Draw() {
    BeginDrawing();

    ClearBackground(WHITE);
    if (gameState != WINNER_GAMEOVER && gameState != TRY_AGAIN) {

        if (gameState != COUNTDOWN) {
            DrawText(TextFormat("%d", playerScore), screenWidth / 4, 20, 40, BLACK);
            DrawText(TextFormat("%d", botScore), 3 * screenWidth / 4, 20, 40, BLACK);

            for (int i = 0; i < GetScreenHeight(); i += 20) {
                DrawRectangle(screenWidth / 2, i, 4, 10, BLACK);
            }

            DrawRectangleRec(playerPaddle, BLACK);
            DrawRectangleRec(botPaddle, BLACK);

            DrawCircle(ball.x + ballRadius, ball.y + ballRadius, ballRadius, BLACK);
        }

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


        DrawText("Game Over", gameOverTextPosition.x, gameOverTextPosition.y, gameOverFontSize, BLACK);

        if (playerScore >= 5) {
            DrawText("Player Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, BLACK);
        } else {
            DrawText("Bot Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, BLACK);
        }

        static int tryAgainTimer = 180;

        tryAgainTimer--;

        if (tryAgainTimer <= 0) {
            gameState = TRY_AGAIN;
        }
    }
    EndDrawing();
}

void PongGameInverted::InitializeGame() {
    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);
}

void PongGameInverted::ResetPositions() {
    playerPaddle = { 20.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    botPaddle = { screenWidth - 40.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    ball = { screenWidth / 2 - 10.0f, screenHeight / 2 - 10.0f, 20.0f, 20.0f };

    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

    lastSpeedIncreaseTime = elapsedTime;
}


void PongGameInverted::UpdateCountdown() {
    countdownTimer--;

    if (countdownTimer > 0) {
        DrawText(TextFormat("%d", countdownTimer / 60 + 1), screenWidth / 2 - 20, screenHeight / 2 - 20, 40, BLACK);
    } else {
        gameState = GAMEPLAY;
        InitializeGame();
    }
}

void PongGameInverted::UpdateTryAgain() {
    int tryAgainFontSize = 50;
    int optionFontSize = 30;

    Vector2 tryAgainTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Try Again?", tryAgainFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 yesOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Yes", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 50.0f };
    Vector2 noOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("No", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 100.0f };

    DrawText("Try Again?", tryAgainTextPosition.x, tryAgainTextPosition.y, tryAgainFontSize, BLACK);

    Color optionColor = BLACK;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(MeasureText("Yes", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = ORANGE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            playerScore = 0;
            botScore = 0;
            gameState = COUNTDOWN;
            InitializeGame();
        }
    }

    DrawText("Yes", yesOptionPosition.x, yesOptionPosition.y, optionFontSize, optionColor);

    optionColor = BLACK;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ noOptionPosition.x, noOptionPosition.y, static_cast<float>(MeasureText("No", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = ORANGE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SetReturnToMenu();
        }
    }

    DrawText("No", noOptionPosition.x, noOptionPosition.y, optionFontSize, optionColor);

    return;
}

void PongGameInverted::CheckGoal() {
    if (ball.x <= 0) {
        botScore++;

        ResetPositions();
        gameState = RESET_GAMEPLAY;

        ballSpeed = { 7.0f, 7.0f };
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

void PongGameInverted::UpdateResetGameplay() {
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

void PongGameInverted::MoveBotPaddle() {
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

void PongGameInverted::MoveBall() {
    ball.x += ballSpeed.x;
    ball.y += ballSpeed.y;

    if (ball.y <= 0 || ball.y + ball.height >= GetScreenHeight()) {
        ballSpeed.y *= -1;
    }
}