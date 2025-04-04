#include "Rigidbody.h"
#include "../Physics.h"

Rigidbody::Rigidbody(GameObject* gameObject) : Component(gameObject) {
}

Rigidbody::~Rigidbody() {
	std::cout << "Rigidbody destroyed" << std::endl;
}

void Rigidbody::Start() {

}
void Rigidbody::Update(float deltaTime) {

}

void Rigidbody::UpdatePhysics(float deltaTime) {
	if (isKinematic) return;
	this->UpdatePosition(deltaTime);
	this->UpdateRotation(deltaTime);
}

void Rigidbody::UpdatePosition(float deltaTime) {
	if (this->useGravity) {
		this->forcesToApply.y += Physics::GetInstance()->GetGravity() * this->mass;
	}

	glm::vec3 acceleration = this->forcesToApply / this->mass;
	this->velocity += acceleration * deltaTime;
	this->velocity *= 1 - this->friction;

	this->forcesToApply = glm::vec3(0);

	Transform* transfrom = this->gameObject->GetTransform();
	transfrom->position += this->velocity * deltaTime;
}

void Rigidbody::UpdateRotation(float deltaTime) {
	return;
	glm::quat rotation = this->gameObject->GetTransform()->rotation;

	float aVelMag = glm::length(this->angularVelocity);
	float aVelMagHalf = aVelMag * 0.5;
	
	float a = cos(aVelMagHalf);
	glm::vec3 bcd = this->angularVelocity / aVelMag;
	bcd *= sin(aVelMagHalf);

	glm::quat angularVel = glm::quat(a, bcd);

	float time = deltaTime * 0.5;

	rotation += time * angularVel * rotation;
	this->gameObject->GetTransform()->rotation = rotation;


	//glm::mat3 matrixRotation = glm::toMat3(rotation);

	//glm::mat3 crossProductMatrix = { 0.0, -angularVelocity.z, angularVelocity.y,
	//					angularVelocity.z, 0.0, -angularVelocity.x,
	//					-angularVelocity.y, angularVelocity.x, 0.0 };

	////crossProductMatrix = glm::transpose(crossProductMatrix);


	////matrixRotation += deltaTime * crossProductMatrix * matrixRotation;
	//matrixRotation += deltaTime * matrixRotation;


	//this->gameObject->GetTransform()->rotation = glm::toQuat(matrixRotation);
}

void Rigidbody::AddForce(glm::vec3 force) {
	this->forcesToApply += force;
}

void Rigidbody::SetUseGravity(bool state) {
	this->useGravity = state;
}