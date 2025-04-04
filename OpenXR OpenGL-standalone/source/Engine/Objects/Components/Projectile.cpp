#include "Projectile.h"

Projectile::Projectile(GameObject* gameObject) : Component(gameObject) {
}

Projectile::~Projectile() {
	std::cout << "Projectile::Deleting" << std::endl;
}

void Projectile::Start() {

}

void Projectile::Update(float deltaTime) {

	bool isNaN = glm::any(glm::isnan(this->gameObject->GetTransform()->position));

	float distanceToOrigin = glm::distance(glm::vec3(0), this->gameObject->GetTransform()->position);
	if (distanceToOrigin > 3.0f || isNaN) {
		Rigidbody* rigidbody = this->gameObject->GetComponent<Rigidbody>();
		rigidbody->isKinematic = true;
		rigidbody->velocity = glm::vec3(0);

		this->gameObject->GetTransform()->position = this->resetPosition;
	}
}