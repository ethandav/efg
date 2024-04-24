#include "Animation.h"

void Circular::run(double time)
{
    position->x = radius * cos(time * speed * direction + startingAngle);
    position->z = radius * sin(time * speed * direction + startingAngle);
}

void Cycle::run(double time)
{
    float cycle = (sin(time * 0.5f) + 1.0f) * 0.5f;
    float result = 
    *property = low + high * cycle;
    //input->pLinear = 0.00f + 0.02f * cycle;  // Ranges from 0.045 to 0.065
    //input->quadratic = 0.0000f + 0.0025f * cycle;  // Ranges from 0.0075 to 0.01
}