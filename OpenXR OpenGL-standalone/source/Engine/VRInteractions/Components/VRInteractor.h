#pragma once

#include "../../Objects/Components/Component.h"
#include "VRInteractable.h"
#include "../../VRInput.h"
#include "../InteractionManager.h"

class VRInteractor : public Component {
private:
	Hand hand = Hand::LEFT;

	SphereCollider* trigger;

	int velocityIndex = 0;
	glm::vec3 velocitys[10];
	glm::vec3 lastPosition;

	bool grabButtonPressed = false;
public:
	VRInteractor(GameObject* gameObject);
	~VRInteractor();

	void Start() override;
	void Update(float deltaTime) override;

	void OnGrab();
	void OnGrabRemove();

	bool CanGrab();

	//Setter
	void SetHand(Hand hand);

	glm::vec3 GetVelocity();

private:
	void BeginGrab();
	void EndGrab();

	void UpdateVelocity();
};