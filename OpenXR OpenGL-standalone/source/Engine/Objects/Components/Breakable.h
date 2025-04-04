#pragma once
#include "Component.h"

#include "Projectile.h"

#include "../../Physics/Components/Collider.h"
#include "../../Physics/Physics.h"

class Breakable : public Component {
private:
	Collider* ownCollider;

	float regenerationTime = 10.0f;

	float elapsedTime = 0.0f;
public:
	Breakable(GameObject* gameObject);
	~Breakable();

	void Start() override;
	void Update(float deltaTime) override;
};