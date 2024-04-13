#include "GameObjects.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

glm::mat4 CreateModelMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
	glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scalingMatrix;
}
