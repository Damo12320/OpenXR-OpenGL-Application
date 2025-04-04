#pragma once
#include "Component.h"

#include "../../VRInput.h"
#include "MeshRenderer.h"
#include "../../../OpenGLComponents/Mesh/MeshImport.h"
#include "../../Materials/MaterialFactory.h"
#include "../../Physics/Components/SphereCollider.h"
#include "../../Physics/Components/Rigidbody.h"
#include "../../VRInteractions/Components/VRInteractable.h"
#include "Projectile.h"

class ProjectileHandSpawner : public Component {
private:
	Hand hand = Hand::LEFT;

	int velocityIndex = 0;
	glm::vec3 velocitys[10];
	glm::vec3 lastPosition;

	bool grabButtonPressed = false;

	GameObject* spawnedBoulder = nullptr;

public:
	ProjectileHandSpawner(GameObject* gameObject);
	~ProjectileHandSpawner();

	void Start() override;
	void Update(float deltaTime) override;

	//Setter
	void SetHand(Hand hand);

private:
	glm::vec3 GetVelocity();

	void BeginGrab();
	void EndGrab();

	void UpdateVelocity();
};