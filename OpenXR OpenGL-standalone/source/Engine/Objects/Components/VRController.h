#pragma once

#include "Component.h"
#include "../../VRInput.h"
#include "../../Materials/Material_SolidColor.h"

class VRController : public Component {
private:
	Hand hand = Hand::LEFT;

	Material_SolidColor* controllerMaterial;
public:
	VRController(GameObject* gameObject);
	~VRController();

	void Start() override;
	void Update(float deltaTime) override;

	void SetHand(Hand hand);
private:
	std::string GetHandString();
};