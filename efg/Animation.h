#pragma once
#include <glm/ext/matrix_transform.hpp>

class Animation
{
public:
	Animation() {};
	virtual void run(glm::vec3& position, double time) = 0;

	float speed = 1.0f;
	glm::vec3 movementDirection = glm::vec3(0.0f, 0.0f, 1.0f);
private:
};

class Circular : public Animation
{
public:
	Circular() {};
	virtual void run(glm::vec3& position, double time);

	float startingAngle = 0.0f;
    float radius = 20.0f;
    int direction = 1;
private:
};
