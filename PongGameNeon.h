// PongGameNeon.h
#ifndef PONGGAMENEON_H
#define PONGGAMENEON_H

#include "PongGame.h"

//inherits from PongGame, this is "Neon" game style


class PongGameNeon : public PongGame {
public:
    PongGameNeon();
    ~PongGameNeon();

    void Update();
    void Draw() override;

    void UpdateCountdown();
    void ResetPositions(); 
    void UpdateTryAgain();
    void CheckGoal();

    virtual void CheckBallCollision();
    virtual void UpdateResetGameplay();

    virtual void MoveBotPaddle();
    virtual void MoveBall();

    virtual void UpdateWinnerGameover();
    virtual void InitializeGame();
};

#endif // PONGGAMENEON_H
