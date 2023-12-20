// PongGameReversal.h
#ifndef PONGGAMEREVERSAL_H
#define PONGGAMEREVERSAL_H

#include "PongGame.h"

//inherits from PongGame, this is "Reversalk" game style


class PongGameReversal : public PongGame {
public:
    PongGameReversal();
    ~PongGameReversal();

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
    
    Color ballColor;
    Rectangle ballPrevPosition;
    void DrawCountdown();
};

#endif // PONGGAMEREVERSAL_H
