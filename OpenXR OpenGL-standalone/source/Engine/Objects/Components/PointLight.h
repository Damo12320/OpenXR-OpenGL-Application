#pragma once

#include "Component.h"

class PointLight : public Component {
private:
	glm::vec3 color;
	float specularStrength;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

public:
	PointLight(GameObject* gameObject);
	~PointLight();

	void Start() override;
	void Update(float deltaTime) override;

	// Setters
	void SetColor(glm::vec3 color);
	void SetSpecularStrength(float specularStrength);

	// Getters
	glm::vec3 GetColor();
	float GetSpecularStrength();
	glm::vec3 GetPosition();

	float GetConstant();
	float GetLinear();
	float GetQuadratic();

};