#pragma once
#include "Collider.h"

struct Bounds {
	glm::vec3 center;
	glm::vec3 halfSize;

	glm::vec3 axis[3];
};

class BoxCollider : public Collider {
private:
	glm::vec3 halfSize;
public:
	BoxCollider(GameObject* gameObject);
	~BoxCollider();

	void Start() override;
	void Update(float deltaTime) override;

	void SetHalfSize(glm::vec3 halfSize);
	Bounds GetBounds();
};