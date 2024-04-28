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
}