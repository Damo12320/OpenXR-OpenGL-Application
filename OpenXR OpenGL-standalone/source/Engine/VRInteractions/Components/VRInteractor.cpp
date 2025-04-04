#include "VRInteractor.h"
#include "../../Physics/Physics.h"
#include "../../../Time.h"

VRInteractor::VRInteractor(GameObject* gameObject) : Component(gameObject) {

}
VRInteractor::~VRInteractor() {
	InteractionManager::GetInstance()->RemoveInteractor(this);
}

void VRInteractor::Start() {
	this->trigger = this->gameObject->GetComponent<SphereCollider>();
	if (this->trigger == nullptr) {
		std::cout << "ERROR::VRInteractor::no collider attached" << std::endl;
		return;
	}

	this->trigger->IsTrigger = true;
}
void VRInteractor::Update(float deltaTime) {
	this->UpdateVelocity();

	bool grabButtonPressedNow = VRInput::GetInstance()->IsHandGrabbing(this->hand);
	if (this->grabButtonPressed == grabButtonPressedNow) return;

	if (this->grabButtonPressed == true && grabButtonPressedNow == false) {
		this->EndGrab();
	}
	else if (this->grabButtonPressed == false && grabButtonPressedNow == true) {
		this->BeginGrab();
	}

	this->grabButtonPressed = grabButtonPressedNow;
}

void VRInteractor::OnGrab() {

}
void VRInteractor::OnGrabRemove() {

}

bool VRInteractor::CanGrab() {
	return true;
}

//Setter
void VRInteractor::SetHand(Hand hand) {
	this->hand = hand;
}

glm::vec3 VRInteractor::GetVelocity() {
	glm::vec3 velocity{};
	//std::cout << "---------------------------------------" << std::endl;
	for (glm::vec3 value : this->velocitys) {
		velocity += value;
		//std::cout << "Velocity: "
		//	<< " |x: " << value.x
		//	<< " |y: " << value.y
		//	<< " |z: " << value.z
		//	<< std::endl;
	}
	//std::cout << "---------------------------------------" << std::endl;
	velocity /= (sizeof(this->velocitys) / sizeof(glm::vec3));
	return velocity;
}


//private
void VRInteractor::BeginGrab() {
	//std::cout << "BeginGrab" << std::endl;

	if (this->trigger == nullptr) {
		std::cout << "ERROR::VRInteractor::trigger is null" << std::endl;
	}

	Collider* otherCollider = nullptr;
	if (!Physics::GetInstance()->Collided(this->trigger, otherCollider)) {
		return;
	}
	if (otherCollider == nullptr) {
		std::cout << "ERROR::VRInteractor::other collider is null" << std::endl;
	}
	VRInteractable* interactable = otherCollider->GetGameObject()->GetComponent<VRInteractable>();

	//VRInteractable* interactable = InteractionManager::GetInstance()->SphereCastInteractables(this->gameObject->GetTransform()->position, this->castRadius);

	if (interactable != nullptr) {
		bool grabbed = InteractionManager::GetInstance()->TryGrab(this, interactable);
		//std::cout << "Grab Object: " << grabbed << std::endl;
	}
}
void VRInteractor::EndGrab() {
	//std::cout << "EndGrab" << std::endl;
	//InteractionManager::GetInstance()->ForceRemoveGrab(this);
	glm::vec3 velocity = this->GetVelocity();
	velocity *= 50.0;
	//std::cout << "Velocity: " 
	//	<< " |x: " << velocity.x
	//	<< " |y: " << velocity.y 
	//	<< " |z: " << velocity.z
	//	<< std::endl;
	InteractionManager::GetInstance()->Throw(this, velocity);
}

void VRInteractor::UpdateVelocity() {
	glm::vec3 velocity = this->gameObject->GetTransform()->GetPosition() - this->lastPosition;
	velocity = velocity / Time::DeltaTime();

	this->lastPosition = this->gameObject->GetTransform()->GetPosition();

	this->velocitys[this->velocityIndex] = velocity;
	this->velocityIndex++;
	if (this->velocityIndex >= sizeof(this->velocitys) / sizeof(glm::vec3)) {
		this->velocityIndex = 0;
	}
}