// Intro.h

#ifndef INTRO_H
#define INTRO_H

#include "raylib.h"

class Intro {
public:
    Intro();
    ~Intro();

    //timer for intro seq.
    float introTimer;
    //flag determines when to show title
    bool showTitle;

    //functions called
    //handling timing and transitions
    void Update();
    //rendering text
    void Draw();

};

#endif // INTRO_H
