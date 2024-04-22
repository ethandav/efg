#include "Animation.h"

void Circular::run(glm::vec3& position, double time)
{
    position.x = radius * cos(time * speed * direction + startingAngle);
    position.z = radius * sin(time * speed * direction + startingAngle);
}