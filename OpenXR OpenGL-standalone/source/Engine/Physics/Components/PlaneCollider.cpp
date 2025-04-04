#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(GameObject* gameObject) : Collider(gameObject) {
}

PlaneCollider::~PlaneCollider() {
	std::cout << "PlaneCollider destroyed" << std::endl;
}

void PlaneCollider::Start() {

}
void PlaneCollider::Update(float deltaTime) {

}

glm::vec3 PlaneCollider::GetNormal() {
	return this->gameObject->GetTransform()->GetUp();
}
glm::vec3 PlaneCollider::GetOrigin() {
	return this->gameObject->GetTransform()->position;
}