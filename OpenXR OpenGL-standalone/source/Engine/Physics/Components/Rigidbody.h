#pragma once
#include "../../Objects/Components/Component.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>


class Rigidbody : public Component {
private:
	bool useGravity = true;

	glm::vec3 forcesToApply;
public:
	float mass = 1;
	float invertedMass = 1;
	float friction = 0.02;

	bool isKinematic = false;

	glm::vec3 velocity;

	glm::vec3 angularVelocity;
public:
	Rigidbody(GameObject* gameObject);
	~Rigidbody();

	void Start() override;
	void Update(float deltaTime) override;

	void UpdatePhysics(float deltaTime);

	void AddForce(glm::vec3 force);

	void SetUseGravity(bool state);

private:
	void UpdatePosition(float deltaTime);
	void UpdateRotation(float deltaTime);
};