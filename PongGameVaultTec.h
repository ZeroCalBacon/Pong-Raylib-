// PongGameVaultTec.h
#ifndef PONGGAMEVAULTTEC_H
#define PONGGAMEVAULTTEC_H

#include "PongGame.h"

//inherits from PongGame, this is "Vault-Tec" game style


class PongGameVaultTec : public PongGame {
public:
    PongGameVaultTec();
    ~PongGameVaultTec();

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

#endif // PONGGAMEVAULTTEC_H
