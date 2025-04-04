#include "Breakable.h"

Breakable::Breakable(GameObject* gameObject) : Component(gameObject) {
}

Breakable::~Breakable() {
	std::cout << "Projectile::Deleting" << std::endl;
}

void Breakable::Start() {
	this->ownCollider = this->gameObject->GetComponent<BoxCollider>();
	if (this->ownCollider == nullptr) {
		std::cout << "ERROR::BREAKABLE::No Collider found" << std::endl;
	}
}

void Breakable::Update(float deltaTime) {
	if (this->ownCollider == nullptr) return;

	if (this->elapsedTime >= this->regenerationTime) {
		this->gameObject->active = true;
		this->elapsedTime = 0.0;
	}

	if (!this->gameObject->active) {
		this->elapsedTime += deltaTime;
		return;
	}

	std::vector<Collider*> otherColliders;
	if (!Physics::GetInstance()->Collided(this->ownCollider, otherColliders)) {
		return;
	}

	for (int i = 0; i < otherColliders.size(); i++) {
		Projectile* proj = otherColliders[i]->GetGameObject()->GetComponent<Projectile>();
		if (proj != nullptr) {
			this->gameObject->active = false;
			//Scene::GetCurrentScene()->DestroyGameObject(this->gameObject);
		}
	}
}