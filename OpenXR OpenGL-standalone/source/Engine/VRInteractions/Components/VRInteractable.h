#pragma once

#include "../../Objects/Components/Component.h"
#include "../InteractionManager.h"

class VRInteractable : public Component {
public:
	bool HasGravity = true;

	VRInteractable(GameObject* gameObject);
	~VRInteractable();

	void Start() override;
	void Update(float deltaTime) override;

	void OnGrab();
	void OnGrabRemove();

	bool CanBeGrabbed();
};