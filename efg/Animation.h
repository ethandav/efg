#pragma once
#include <glm/ext/matrix_transform.hpp>

class Animation
{
public:
	Animation() {};
	virtual void run(double time) = 0;

	float speed = 1.0f;
private:
};

class Circular : public Animation
{
public:
	Circular(glm::vec3* position) : position(position) {};
	virtual void run(double time);

	float startingAngle = 0.0f;
    float radius = 20.0f;
    int direction = 1;
	glm::vec3* position = nullptr;
private:
};

class Cycle : public Animation
{
public:
	Cycle(float* property, float low, float high) : property(property), low(low), high(high) {};
	virtual void run(double time);

	float low = 0.0f;
	float high = 0.0f;
	float* property = nullptr;
};
