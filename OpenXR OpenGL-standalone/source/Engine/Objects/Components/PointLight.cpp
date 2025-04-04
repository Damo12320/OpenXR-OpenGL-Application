#include "PointLight.h"

PointLight::PointLight(GameObject* gameObject) : Component(gameObject) {
	this->color = glm::vec3(1.0f);
	this->specularStrength = 1.0f;
}

PointLight::~PointLight() {
	std::cout << "POINTLIGHT::Deleting" << std::endl;
}

void PointLight::Start() {

}

void PointLight::Update(float deltaTime) {

}


// Setters
void PointLight::SetColor(glm::vec3 color) {
	this->color = color;
}

void PointLight::SetSpecularStrength(float specularStrength) {
	this->specularStrength = specularStrength;
}


// Getters
glm::vec3 PointLight::GetColor() {
	return this->color;
}

float PointLight::GetSpecularStrength() {
	return this->specularStrength;
}

glm::vec3 PointLight::GetPosition() {
	return this->gameObject->GetTransform()->GetPosition();
}

float PointLight::GetConstant() {
	return this->constant;
}

float PointLight::GetLinear() {
	return this->linear;
}

float PointLight::GetQuadratic() {
	return this->quadratic;
}