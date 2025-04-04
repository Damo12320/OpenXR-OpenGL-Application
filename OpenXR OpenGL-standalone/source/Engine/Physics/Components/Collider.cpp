#include "Collider.h"

Collider::Collider(GameObject* gameObject) : Component(gameObject) {

}

Collider::~Collider() {
	std::cout << "Collider destroyed" << std::endl;
}

void Collider::Start() {

}
void Collider::Update(float deltaTime) {

}