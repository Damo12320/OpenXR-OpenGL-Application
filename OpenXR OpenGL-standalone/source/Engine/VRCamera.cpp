#include "VRCamera.h"
VRCamera::VRCamera(float nearZ, float farZ) {
	this->nearZ = nearZ;
	this->farZ = farZ;

	this->CameraWorldOffset = glm::vec3(0, 0, 0);
}


glm::mat4 VRCamera::CreateViewMatrix(XrView view) {
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, this->GetCameraPosition(view));

	glm::quat cameraOrientation = glm::quat(view.pose.orientation.w, view.pose.orientation.x, view.pose.orientation.y, view.pose.orientation.z);
	glm::mat4 RotationMatrix = glm::toMat4(cameraOrientation);

	glm::mat4 cameraView = translation * RotationMatrix;
	cameraView = glm::inverse(cameraView);

	return cameraView;
}

glm::mat4 VRCamera::CreateProjectionMatrix(XrView view) {
	glm::mat4 cameraProjection = glm::mat4(1.0f);
	CreateProjectionFov(&cameraProjection, view.fov, this->nearZ, this->farZ);

	return cameraProjection;
}

glm::vec3 VRCamera::GetCameraPosition(XrView view) {
	glm::vec3 posePosition = glm::vec3(view.pose.position.x, view.pose.position.y, view.pose.position.z);
	return this->CameraWorldOffset + posePosition;
}