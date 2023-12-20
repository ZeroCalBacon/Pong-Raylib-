// PongGameTron.h
#ifndef PONGGAMETRON_H
#define PONGGAMETRON_H

#include "PongGame.h"

//inherits from PongGame, this is "Tron" game style


//enum for ball color....edit later
enum BallColor {
    BALL_COLOR_WHITE,
    BALL_COLOR_SKYBLUE,
    BALL_COLOR_ORANGE
};

class PongGameTron : public PongGame {
public:
    PongGameTron();
    ~PongGameTron();

    void Update();
    void Draw() override;

    void UpdateCountdown();
    void ResetPositions(); 
    void UpdateTryAgain();
    void CheckGoal();
    virtual void UpdateResetGameplay();

    virtual void MoveBotPaddle();
    virtual void MoveBall();

    virtual void InitializeGame();
    virtual void CheckBallCollision();

private:
    Vector2 ballTrail[50];
    int trailLength = 20;
    float trailDuration = 1.0f;

    BallColor ballColor;
};

#endif // PONGGAMETRON_H
