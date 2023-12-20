#include "MathFunction.h"

//lerp linear interpolation function
//needed for bot paddle for smooth movement
//based on following the ball
//do not delete
float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}
