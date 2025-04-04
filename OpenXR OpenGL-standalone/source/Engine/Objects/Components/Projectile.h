#pragma once
#include "Component.h"
#include "../../Physics/Components/Rigidbody.h"

class Projectile : public Component {
public:
	glm::vec3 resetPosition = glm::vec3(0, 1.5, 0);
public:
	Projectile(GameObject* gameObject);
	~Projectile();

	void Start() override;
	void Update(float deltaTime) override;
};