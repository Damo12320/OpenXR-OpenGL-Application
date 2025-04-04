#include "Component.h"

Component::Component(GameObject* gameObject) {
	this->gameObject = gameObject;
}

Component::~Component() {
	std::cout << "COMPONENT::Deleting" << std::endl;
}

GameObject* Component::GetGameObject() {
	return this->gameObject;
}