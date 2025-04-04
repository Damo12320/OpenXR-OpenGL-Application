#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <iostream>

class Transform {
public:
	glm::vec3 position{};
	glm::quat rotation{};
	glm::vec3 scale{1.0f, 1.0f, 1.0f};

	~Transform();

	void GetModelMatrix(glm::mat4& model);

	void SetEulerRotationDeg(glm::vec3 eulerRotation);

	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();

	glm::vec3 GetForward();
	glm::vec3 GetUp();
	glm::vec3 GetRight();
};