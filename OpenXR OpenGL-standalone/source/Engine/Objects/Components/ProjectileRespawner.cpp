#include "ProjectileRespawner.h"

ProjectileRespawner::ProjectileRespawner(GameObject* gameObject) : Component(gameObject) {

}
ProjectileRespawner::~ProjectileRespawner() {

}

void ProjectileRespawner::Start() {
	this->ownCollider = this->gameObject->GetComponent<PlaneCollider>();
	if (this->ownCollider == nullptr) {
		std::cout << "ERROR::ProjectileRespawner::No Collider found" << std::endl;
	}
}
void ProjectileRespawner::Update(float deltaTime) {
	if (this->ownCollider == nullptr) return;

	std::vector<Collider*> otherColliders;
	if (!Physics::GetInstance()->Collided(this->ownCollider, otherColliders)) {
		return;
	}

	for (int i = 0; i < otherColliders.size(); i++) {
		Projectile* proj = otherColliders[i]->GetGameObject()->GetComponent<Projectile>();
		if (proj != nullptr) {
			Rigidbody* rigidbody = proj->GetGameObject()->GetComponent<Rigidbody>();
			rigidbody->isKinematic = true;
			rigidbody->velocity = glm::vec3(0);

			rigidbody->GetGameObject()->GetTransform()->position = proj->resetPosition;
		}
	}
}