#pragma once

#include "Component.h"

class SpotLight : public Component {
private:
	glm::vec3 color;
	float openingAngle;

	float specularStrength;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
public:
	SpotLight(GameObject* gameObject);
	~SpotLight();

	void Start() override;
	void Update(float deltaTime) override;

	// Setters
	void SetColor(glm::vec3 color);
	void SetOpeningAngle(float openingAngle);
	void SetSpecularStrength(float specularStrength);

	// Getters
	glm::vec3 GetColor();
	float GetOpeningAngle();
	float GetSpecularStrength();

	glm::vec3 GetPosition();
	glm::vec3 GetDirection();

	float GetConstant();
	float GetLinear();
	float GetQuadratic();
};