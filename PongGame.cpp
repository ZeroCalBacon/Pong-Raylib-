// PongGame.cpp
#include "PongGame.h"
#include "MathFunction.h"
#include <math.h>


//this is parent definition of PongGame, also called "Default"
//other gamestyles inherit from this
//use this to read comments on game logic | functions

//NOTE: for other game styles, they only change aesthetical aspects not game mechanics.

PongGame::PongGame() {
    //initialize game variables here
    screenWidth = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());

    playerPaddle = { 20.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    botPaddle = { screenWidth - 40.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };

    ball = { screenWidth / 2 - 10.0f, screenHeight / 2 - 10.0f, 20.0f, 20.0f };
    ballRadius = 12.0f;

    paddleSpeed = 10.0f;
    ballSpeed = { 8.0f, 8.0f };

    playerScore = 0;
    botScore = 0;

    gameState = COUNTDOWN;
    //3 sec
    countdownTimer = 180;
    returnToMenu = false;
    
    InitializeGame();
}

//use for clean up | deconstruct.
PongGame::~PongGame() {
    //cleanup  dynamic mem. | resources here
}

//used for updating logic 
void PongGame::Update() {
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

    //update elapsed time
    elapsedTime += GetFrameTime();

    //ensures gamestates logic is only in the GAMEPLAY state
    if (gameState == GAMEPLAY) {
        MovePlayerPaddle();
        MoveBotPaddle();
        MoveBall();
        CheckBallCollision();
        CheckGoal();
    }

    //check if 10 seconds have passed since the last speed increase
    if (elapsedTime - lastSpeedIncreaseTime >= 10.0f) {
        //normalize the ball velocity vector
        float magnitude = sqrt(ballSpeed.x * ballSpeed.x + ballSpeed.y * ballSpeed.y);

        if (magnitude > 0.0f) {
            //calc, the normalized direction
            float normalizedX = ballSpeed.x / magnitude;
            float normalizedY = ballSpeed.y / magnitude;

            //update the ball speed w/o changing its direction
            ballSpeed.x = normalizedX * 7.0f;
            ballSpeed.y = normalizedY * 7.0f;
        }

        //update the last speed increase time
        lastSpeedIncreaseTime = elapsedTime;
    }
}

//used for drawing objs.
void PongGame::Draw() {
    BeginDrawing();

    ClearBackground(BLACK);
    if (gameState != WINNER_GAMEOVER && gameState != TRY_AGAIN) {
        if (gameState != COUNTDOWN) {
            //draws scores
            DrawText(TextFormat("%d", playerScore), screenWidth / 4, 20, 40, WHITE);
            DrawText(TextFormat("%d", botScore), 3 * screenWidth / 4, 20, 40, WHITE);

            //draws the dotted line in the middle
            for (int i = 0; i < GetScreenHeight(); i += 20) {
                DrawRectangle(screenWidth / 2, i, 4, 10, WHITE);
            }

            //draws the game objs. (paddles and ball)
            DrawRectangleRec(playerPaddle, WHITE);
            DrawRectangleRec(botPaddle, WHITE);

            //draws the ball
            DrawCircle(ball.x + ballRadius, ball.y + ballRadius, ballRadius, WHITE);
        }
    }
    EndDrawing();
}

//used for initial obj var
void PongGame::InitializeGame() {
    //sets up the initial game state
    countdownTimer = 180;

    //randomize ball direction after countdown
    //always diaganol
    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);
}

//used for reset after score
void PongGame::ResetPositions() {
    //reset positions of game objs.
    playerPaddle = { 20.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    botPaddle = { screenWidth - 40.0f, screenHeight / 2 - 40.0f, 20.0f, 80.0f };
    ball = { screenWidth / 2 - 10.0f, screenHeight / 2 - 10.0f, 20.0f, 20.0f };

    ballSpeed = { 8.0f, 8.0f };

    //randomize ball direction at the start of each round
    int randomDirectionX = GetRandomValue(0, 1);
    int randomDirectionY = GetRandomValue(0, 1);

    ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
    ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

    lastSpeedIncreaseTime = elapsedTime;
}

//used for drawing countdown
void PongGame::UpdateCountdown() {
    countdownTimer--;

    //draws countdown timer (3 sec)
    if (countdownTimer > 0) {
        DrawText(TextFormat("%d", countdownTimer / 60 + 1), screenWidth / 2 - 20, screenHeight / 2 - 20, 40, WHITE);
    } else {
        gameState = GAMEPLAY;
        InitializeGame();
    }
}

//used for score transition
void PongGame::UpdateGameplay() {
    //check for score conditions and transition to RESET_GAMEPLAY if needed
    if (playerScore >= 5 || botScore >= 5) {
        gameState = WINNER_GAMEOVER;
    }
    
}

//used for try again state
void PongGame::UpdateTryAgain() {
    //text sizes
    int tryAgainFontSize = 50;
    int optionFontSize = 30;

    //calc. text positions
    Vector2 tryAgainTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Try Again?", tryAgainFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 yesOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Yes", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 50.0f };
    Vector2 noOptionPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("No", optionFontSize)) / 2, static_cast<float>(screenHeight) / 2 + 100.0f };

    //draw try again text
    DrawText("Try Again?", tryAgainTextPosition.x, tryAgainTextPosition.y, tryAgainFontSize, WHITE);

    //initial text color
    Color optionColor = WHITE;

    //check if the mouse is over the 'Yes' option
    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(MeasureText("Yes", optionFontSize)), static_cast<float>(optionFontSize) })) {
        //change color to skyblue
        optionColor = SKYBLUE;

        //check for left mouse button click on 'Yes'
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            //reset the entire game
            playerScore = 0;
            botScore = 0;
            gameState = COUNTDOWN;
            InitializeGame();
        }
    }

    //draw 'Yes' option with hover effect
    DrawText("Yes", yesOptionPosition.x, yesOptionPosition.y, static_cast<float>(optionFontSize), optionColor);

    //reset option color
    optionColor = WHITE;

    //check if the mouse is over the 'No' option
    if (CheckCollisionPointRec(GetMousePosition(), Rectangle{ noOptionPosition.x, noOptionPosition.y, static_cast<float>(MeasureText("No", optionFontSize)), static_cast<float>(optionFontSize) })) {
        optionColor = SKYBLUE;

        //check for left mouse button click on 'No'
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
             //brings user back to menu
            SetReturnToMenu();
        }
    }

    //draws 'No' option with hover effect
    DrawText("No", noOptionPosition.x, noOptionPosition.y, static_cast<float>(optionFontSize), optionColor);

    return;
}

//used for updating the ResetPos().
void PongGame::UpdateResetGameplay() {
    countdownTimer--;

    if (countdownTimer <= 0) {
        //reset positions and transition to GAMEPLAY state after countdown
        ResetPositions();
        gameState = GAMEPLAY;

        InitializeGame();

        int randomDirectionX = GetRandomValue(0, 1);
        int randomDirectionY = GetRandomValue(0, 1);

        ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
        ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

    }
}

//used for checking who scores
void PongGame::CheckGoal() {
    if (ball.x <= 0) {
        botScore++;

        //reset positions and transition to RESET_GAMEPLAY state after scoring
        ResetPositions();
        gameState = RESET_GAMEPLAY;

        int randomDirectionX = GetRandomValue(0, 1);
        int randomDirectionY = GetRandomValue(0, 1);

        ballSpeed.x = (randomDirectionX == 0) ? -fabs(ballSpeed.x) : fabs(ballSpeed.x);
        ballSpeed.y = (randomDirectionY == 0) ? -fabs(ballSpeed.y) : fabs(ballSpeed.y);

        //reset ball speed to initial values
        ballSpeed = { 8.0f, 8.0f };
    }

    if (ball.x + ballRadius * 2 >= GetScreenWidth()) {
        playerScore++;

        //reset positions and transition to RESET_GAMEPLAY state after scoring
        ResetPositions();
        gameState = RESET_GAMEPLAY;
    }

    //scoring, anyone who reaches 5 pts first
    if (playerScore >= 5 || botScore >= 5) {
        gameState = WINNER_GAMEOVER;
    }

    if (gameState == RESET_GAMEPLAY) {
        //initialize the countdown timer when transitioning to RESET_GAMEPLAY
        countdownTimer = 120;
    }
}

//used for moving player paddle 
void PongGame::MovePlayerPaddle() {
    if (IsKeyDown(KEY_W) && playerPaddle.y > 0) {
        playerPaddle.y -= paddleSpeed;
    }

    if (IsKeyDown(KEY_S) && playerPaddle.y + playerPaddle.height < GetScreenHeight()) {
        playerPaddle.y += paddleSpeed;
    }
}

//used for moving bot paddle
void PongGame::MoveBotPaddle() {
    const float aiSpeed = 10.0f;

    float targetY = ball.y + ballRadius - botPaddle.height / 2;

    if (GetRandomValue(0, 100) < 90) {
        //uses lerp funct, smoothen movement
        botPaddle.y = Lerp(botPaddle.y, targetY, aiSpeed * GetFrameTime());
    } else {
        //randomly miss the bounce by changing the speed
        //adjust later...
        float randomSpeed = GetRandomValue(-1, 1);
        botPaddle.y += randomSpeed;
    }

    //ensure the bot paddle stays within the screen bounds
    if (botPaddle.y < 0) {
        botPaddle.y = 0;
    } else if (botPaddle.y + botPaddle.height > screenHeight) {
        botPaddle.y = screenHeight - botPaddle.height;
    }
}

//used for moving ball
void PongGame::MoveBall() {
    //update ball position here
    ball.x += ballSpeed.x;
    ball.y += ballSpeed.y;

    //bounces off the top and bottom
    if (ball.y <= 0 || ball.y + ball.height >= GetScreenHeight()) {
        ballSpeed.y *= -1;
    }
}

//used for ball collision
void PongGame::CheckBallCollision() {
    Vector2 ballCenter = { ball.x + ballRadius, ball.y + ballRadius };

    //for player
    if (CheckCollisionPointRec(ballCenter, playerPaddle)) {
        //reverse ball direction on collision with player paddle
        //this always make ball move towards positive x direction
        ballSpeed.x = fabs(ballSpeed.x); 

        //adjust ball bounce based on the paddle's movement, going up or down
        //little wonky, must be done split sec before - update for longer period...
        float relativeIntersectY = (playerPaddle.y + playerPaddle.height / 2) - ballCenter.y;
        float normalizedRelativeIntersectionY = (relativeIntersectY / (playerPaddle.height / 2));
        //adjust the ball vel. based on the paddle movement direction
        ballSpeed.y = normalizedRelativeIntersectionY * fabs(ballSpeed.x);

        //increase ball speed by 2 if the paddle is moving up
        if (IsKeyDown(KEY_W)) {
            ballSpeed.y -= 2.0f;
        }
        // Increase ball speed by 2 if the paddle is moving down
        else if (IsKeyDown(KEY_S)) {
            ballSpeed.y += 2.0f;
        }

        //ensure that the ball always moves towards the positive x direction
        if (ballSpeed.x < 0) {
            ballSpeed.x *= -2;
        }
    }

    //for bot
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

//used for game over | winner texts
void PongGame::UpdateWinnerGameover() {
    int gameOverFontSize = 60;
    int winnerFontSize = 50;

    //calc. text positions using vector2
    Vector2 gameOverTextPosition = { static_cast<float>(screenWidth) / 2 - static_cast<float>(MeasureText("Game Over", gameOverFontSize)) / 2, static_cast<float>(screenHeight) / 2 - 50.0f };
    Vector2 winnerTextPosition;

    if (playerScore >= 5) {
        winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Player Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
    } else {
        winnerTextPosition = { static_cast<float>(screenWidth) / 2 - MeasureText("Bot Won", winnerFontSize) / 2, static_cast<float>(screenHeight) / 2 };
    }

    //draw game over and winner text
    DrawText("Game Over", gameOverTextPosition.x, gameOverTextPosition.y, gameOverFontSize, WHITE);
    if (playerScore >= 5) {
        DrawText("Player Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, WHITE);
    } else {
        DrawText("Bot Won", winnerTextPosition.x, winnerTextPosition.y, winnerFontSize, WHITE);
    }

    //reset timer
    static int tryAgainTimer = 180;

    tryAgainTimer--;

    if (tryAgainTimer <= 0) {
        gameState = TRY_AGAIN;
    }

    return;
}
