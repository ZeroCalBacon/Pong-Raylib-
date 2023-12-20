// PongGameInverted.cpp
#include "PongGameTron.h"
#include "MathFunction.h"
#include <math.h>

//inherits from PongGame, this is "Tron" game style
//changes player objs to skyblue, changes bot obj to orange
//changes ball to have trail, neutral white

PongGameTron::PongGameTron() : PongGame() {
    screenWidth = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());

    paddleSpeed = 10.0f;
    ballSpeed = { 8.0f, 8.0f };

    gameState = COUNTDOWN; 
    InitializeGame();

}

PongGameTron::~PongGameTron() {
}

void PongGameTron::Update() {
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
void PongGameTron::Draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    if (gameState != WINNER_GAMEOVER && gameState != TRY_AGAIN) {

        if (gameState != COUNTDOWN) {
            DrawText(TextFormat("%d", playerScore), screenWidth / 4, 20, 40, SKYBLUE);
            DrawText(TextFormat("%d", botScore), 3 * screenWidth / 4, 20, 40, ORANGE);

            for (int i = 0; i < GetScreenHeight(); i += 20) {
                DrawRectangle(screenWidth / 2, i, 4, 10, WHITE);
            }

            DrawRectangleRec(playerPaddle, SKYBLUE);
            DrawRectangleRec(botPaddle, ORANGE);

            //draw the ball trail
            for (int i = 0; i < trailLength; ++i) {
                //calc. alpha based on distance
                float alpha = (trailLength - i) / static_cast<float>(trailLength);
                DrawCircleV(ballTrail[i], ballRadius, Fade(WHITE, alpha));
            }

            //draw the current ball position
            DrawCircle(ball.x + ballRadius, ball.y + ballRadius, ballRadius, WHITE);

            //update the ball trail positions
            for (int i = trailLength - 1; i > 0; --i) {
                ballTrail[i] = ballTrail[i - 1];
            }

            ballTrail[0] = { ball.x + ballRadius, ball.y + ballRadius };
        }
    }

    EndDrawing();
}

void PongGameTron::InitializeGame() {
    countdownTimer = 180;

    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);
}

void PongGameTron::ResetPositions() {
    playerPaddle = { 20.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    botPaddle = { screenWidth - 40.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    ball = { screenWidth / 2 - 10.0f, screenHeight / 2 - 10.0f, 20.0f, 20.0f };

    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

    lastSpeedIncreaseTime = elapsedTime;
}


void PongGameTron::UpdateCountdown() {
    countdownTimer--;

    if (countdownTimer > 0) {
        DrawText(TextFormat("%d", countdownTimer / 60 + 1), screenWidth / 2 - 20, screenHeight / 2 - 20, 40, WHITE);
    } else {
        gameState = GAMEPLAY;
        InitializeGame();
    }
}

void PongGameTron::UpdateTryAgain() {
    int tryAgainFontSize = 50;
    int optionFontSize = 30;

    Vector2 tryAgainTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Try Again?", tryAgainFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 yesOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Yes", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 50.0f };
    Vector2 noOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("No", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 100.0f };
    
    DrawText("Try Again?", tryAgainTextPosition.x, tryAgainTextPosition.y, tryAgainFontSize, WHITE);

    Color optionColor = WHITE;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(MeasureText("Yes", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = SKYBLUE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            playerScore = 0;
            botScore = 0;
            gameState = COUNTDOWN;
            InitializeGame();
        }
    }

    DrawText("Yes", yesOptionPosition.x, yesOptionPosition.y, optionFontSize, optionColor);

    optionColor = WHITE;

    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ noOptionPosition.x, noOptionPosition.y, static_cast<float>(MeasureText("No", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = ORANGE;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SetReturnToMenu();
        }
    }

    DrawText("No", noOptionPosition.x, noOptionPosition.y, optionFontSize, optionColor);

    return;
}

void PongGameTron::CheckGoal() {
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

void PongGameTron::UpdateResetGameplay() {
    countdownTimer--;

    if (countdownTimer <= 0) {
        InitializeGame();
        gameState = GAMEPLAY;
        countdownTimer = 60;

        ballSpeed = { 7.0f, 7.0f };
    }
}

void PongGameTron::MoveBotPaddle() {
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

void PongGameTron::MoveBall() {
    ball.x += ballSpeed.x;
    ball.y += ballSpeed.y;

    if (ball.y <= 0 || ball.y + ball.height >= GetScreenHeight()) {
        ballSpeed.y *= -1;
    }
}

void PongGameTron::CheckBallCollision() {
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

