// PongGameRainbow.h
#ifndef PONGGAMERAINBOW_H
#define PONGGAMERAINBOW_H

#include "PongGame.h"

//inherits from PongGame, this is "Rainbow" game style


class PongGameRainbow : public PongGame {
public:
    PongGameRainbow();
    ~PongGameRainbow();

    void Update();
    void Draw() override;

    void UpdateCountdown();
    void ResetPositions(); 
    void UpdateTryAgain();
    void CheckGoal();
    virtual void UpdateResetGameplay();

    virtual void MoveBotPaddle();
    virtual void MoveBall();

    virtual void CheckBallCollision();
    virtual void InitializeGame();
};

#endif // PONGGAMERAINBOW_H
