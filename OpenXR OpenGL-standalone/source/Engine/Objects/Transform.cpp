#include "Transform.h"

Transform::~Transform() {
	std::cout << "TRANSFORM::Deleting" << std::endl;
}

void Transform::GetModelMatrix(glm::mat4& model) {
	/*model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = model * glm::toMat4(rotation);
	model = glm::scale(model, scale);*/

	model = glm::translate(glm::mat4(1.0f), position) * 
		glm::toMat4(rotation) * 
		glm::scale(glm::mat4(1.0f), scale);
}



void Transform::SetEulerRotationDeg(glm::vec3 eulerRotation) {
	this->rotation = glm::quat(glm::radians(eulerRotation));
}

glm::vec3 Transform::GetPosition() {
	return this->position;
}

glm::quat Transform::GetRotation() {
	return this->rotation;
}

glm::vec3 Transform::GetScale() {
	return this->scale;
}

glm::vec3 Transform::GetForward() {
	//might be wrong (Copilot said it this way)
	//the possibility, that this is true, is nearly 100%
	//return glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) * glm::toMat3(rotation));
	return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 Transform::GetUp() {
	return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
	//return glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f) * glm::toMat3(rotation));
}

glm::vec3 Transform::GetRight() {
	return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
}