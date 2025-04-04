#pragma once
#include "Collider.h"

class PlaneCollider : public Collider {
public:
	PlaneCollider(GameObject* gameObject);
	~PlaneCollider();

	void Start() override;
	void Update(float deltaTime) override;

	glm::vec3 GetNormal();
	glm::vec3 GetOrigin();
};