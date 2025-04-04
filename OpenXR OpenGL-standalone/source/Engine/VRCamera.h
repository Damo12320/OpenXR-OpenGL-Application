#pragma once

#include "../KhronosWrapper/OpenGLHelper.h"

#include <openxr/openxr.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class VRCamera {
private:
	float nearZ;
	float farZ;

public:
	glm::vec3 CameraWorldOffset;
public:
	VRCamera(float nearZ = 0.05f, float farZ = 1000.0f);

	glm::mat4 CreateViewMatrix(XrView view);
	glm::mat4 CreateProjectionMatrix(XrView view);

	glm::vec3 GetCameraPosition(XrView view);
};