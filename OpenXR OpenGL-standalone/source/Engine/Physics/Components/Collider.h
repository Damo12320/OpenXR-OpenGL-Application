#pragma once
#include "../../Objects/Components/Component.h"

class Collider : public Component {
public:
	bool IsTrigger = false;

	float elasticity = 1.0;
public:
	Collider(GameObject* gameObject);
	~Collider() override;

	void Start() override;
	void Update(float deltaTime) override;
};