// PongGameInverted.h
#ifndef PONGGAMEINVERTED_H
#define PONGGAMEINVERTED_H

#include "PongGame.h"

//inherits from PongGame, this is "Inverted" game style

class PongGameInverted : public PongGame {
public:
    PongGameInverted();
    ~PongGameInverted();

    void Update();
    void Draw() override;

    void UpdateCountdown();
    void ResetPositions(); 

    void UpdateTryAgain();
    void CheckGoal();

    virtual void InitializeGame();
    virtual void UpdateResetGameplay();

    virtual void MoveBotPaddle();
    virtual void MoveBall();
};

#endif // PONGGAMEINVERTED_H
