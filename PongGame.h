// PongGame.h
#ifndef PONG_GAME_H
#define PONG_GAME_H

#include "raylib.h"

//this is parent declaration of PongGame, also called "Default"
//other gamestyles inherit from this 

//used for transitioning
enum GameState {
    COUNTDOWN,
    GAMEPLAY,
    RESET_GAMEPLAY,
    WINNER_GAMEOVER,
    TRY_AGAIN,
    BACK_TO_MENU
};

//declare PongGame var | objs
class PongGame {
public:
    int screenWidth;
    int screenHeight;

    PongGame();
    virtual ~PongGame(); 

    virtual void Update();
    virtual void Draw();

    float paddleSpeed;
    Vector2 ballSpeed;

    float elapsedTime = 0.0f;
    float lastSpeedIncreaseTime = 0.0f;

    Rectangle playerPaddle;
    Rectangle botPaddle;

    Rectangle ball;
    float ballRadius;

    int playerScore;
    int botScore;

    void MovePlayerPaddle();
    virtual void MoveBotPaddle();

    void MoveBall();

    virtual void CheckBallCollision();
    void CheckGoal();

    void ResetPositions();

    GameState gameState;
    int countdownTimer;

    virtual void InitializeGame();
    void UpdateCountdown();
    void UpdateGameplay();
    virtual void UpdateResetGameplay();
    virtual void UpdateWinnerGameover();
    void UpdateTryAgain();

    bool returnToMenu; 

    void ResetGameState() {
        returnToMenu = false;
    }
    
    bool ShouldReturnToMenu() const {
        return gameState == BACK_TO_MENU;
    }

    void SetReturnToMenu() {
        gameState = BACK_TO_MENU;
    }
};

#endif // PONG_GAME_H