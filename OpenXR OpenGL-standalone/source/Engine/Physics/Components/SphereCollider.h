#pragma once
#include "Collider.h"

class SphereCollider : public Collider {
private:
	float radius = 0.5;
public:
	SphereCollider(GameObject* gameObject);
	~SphereCollider();

	void Start() override;
	void Update(float deltaTime) override;

	void SetRadius(float radius);
	float GetRadius();
	glm::vec3 GetCenter();
};