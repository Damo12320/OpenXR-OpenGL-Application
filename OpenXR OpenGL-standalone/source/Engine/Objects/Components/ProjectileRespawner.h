#pragma once
#include "Component.h"

#include "Projectile.h"

#include "../../Physics/Components/Collider.h"
#include "../../Physics/Physics.h"

class ProjectileRespawner : public Component {
private:
	Collider* ownCollider;

public:
	ProjectileRespawner(GameObject* gameObject);
	~ProjectileRespawner();

	void Start() override;
	void Update(float deltaTime) override;

};