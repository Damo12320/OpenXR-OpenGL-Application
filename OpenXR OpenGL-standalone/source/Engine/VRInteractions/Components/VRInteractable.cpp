#include "VRInteractable.h"

VRInteractable::VRInteractable(GameObject* gameObject) : Component(gameObject) {

}
VRInteractable::~VRInteractable() {
	InteractionManager::GetInstance()->RemoveInteractable(this);
}

void VRInteractable::Start() {

}
void VRInteractable::Update(float deltaTime) {
	glm::vec3 position;
	if (InteractionManager::GetInstance()->GetNewPosition(this, position)) {
		this->gameObject->GetTransform()->position = position;
	}

	glm::quat rotation;
	if (InteractionManager::GetInstance()->GetNewRotation(this, rotation)) {
		this->gameObject->GetTransform()->rotation = rotation;
	}
}

void VRInteractable::OnGrab() {

}
void VRInteractable::OnGrabRemove() {

}

bool VRInteractable::CanBeGrabbed() {
	return true;
}
