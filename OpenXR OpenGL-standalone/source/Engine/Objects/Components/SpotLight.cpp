#include "SpotLight.h"

SpotLight::SpotLight(GameObject* gameObject) : Component(gameObject) {
	this->color = glm::vec3(1.0f);
	this->openingAngle = 30.0f;
	this->specularStrength = 1.0f;
}

SpotLight::~SpotLight() {
	std::cout << "SPOTLIGHT::Deleting" << std::endl;
}


void SpotLight::Start() {

}

void SpotLight::Update(float deltaTime) {

}


// Setters
void SpotLight::SetColor(glm::vec3 color) {
	this->color = color;
}

void SpotLight::SetOpeningAngle(float openingAngle) {
	this->openingAngle = openingAngle;
}

void SpotLight::SetSpecularStrength(float specularStrength) {
	this->specularStrength = specularStrength;
}


// Getters
glm::vec3 SpotLight::GetColor() {
	return this->color;
}

float SpotLight::GetOpeningAngle() {
	return this->openingAngle;
}

float SpotLight::GetSpecularStrength() {
	return this->specularStrength;
}

glm::vec3 SpotLight::GetPosition() {
	return this->gameObject->GetTransform()->GetPosition();
}

glm::vec3 SpotLight::GetDirection() {
	return this->gameObject->GetTransform()->GetForward();
}

float SpotLight::GetConstant() {
	return this->constant;
}

float SpotLight::GetLinear() {
	return this->linear;
}

float SpotLight::GetQuadratic() {
	return this->quadratic;
}