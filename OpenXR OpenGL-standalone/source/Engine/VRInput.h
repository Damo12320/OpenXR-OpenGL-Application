#pragma once

#include "../KhronosWrapper/OpenXrActionSetManager.h"
#include "../Time.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <iostream>

struct VRInputTransform {
	glm::vec3 position;
	glm::quat rotation;

	glm::vec3 Forward() {
		return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::vec3 Up() {
		return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Right() {
		return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
	}
};

enum Hand {
	LEFT = 0,
	RIGHT = 1
};

class VRInput {
private:
	static VRInput* instance;

	glm::vec3 playerPosition = glm::vec3(0);

	VRInputTransform handTransform[2] = { {glm::vec3(0), glm::quat()} , {glm::vec3(0), glm::quat()} };
	VRInputTransform headTransform;

	float handGrabValue[2] = {0, 0};
	float handSelectValue[2] = { 0, 0 };
	glm::vec2 handThumbstickValue[2] = { glm::vec2(0), glm::vec2(0)};

	VRInput() = default;
public:
	static VRInput* GetInstance();

	void Update(OpenXrActionSetManager* actionsetManager);
	void UpdatePlayerPosition();
	void UpdateHeadTransform(std::vector<XrView>* views, int viewCount);

	VRInputTransform GetHandTransform(Hand hand);
	VRInputTransform GetHeadTransform();

	glm::vec3 GetPlayerPosition();

	float GetHandGrabValue(Hand hand);
	float GetHandSelectValue(Hand hand);
	glm::vec2 GetHandThumbstickValue(Hand hand);

	bool IsHandGrabbing(Hand hand);
	bool IsHandSelecting(Hand hand);
};