#include "BoxCollider.h"

#include "../../Objects/Components/MeshRenderer.h"
#include "../../Materials/Material_SolidColor.h"
#include "../Physics.h"

BoxCollider::BoxCollider(GameObject* gameObject) : Collider(gameObject) {
	this->halfSize = glm::vec3(0.5);
}

BoxCollider::~BoxCollider() {
	std::cout << "BoxCollider destroyed" << std::endl;
}

void BoxCollider::Start() {

}
void BoxCollider::Update(float deltaTime) {
	//MeshRenderer* meshRenderer = this->gameObject->GetComponent<MeshRenderer>();
	//if (meshRenderer == nullptr) {
	//	return;
	//}
	//
	//Material* material = meshRenderer->GetMaterial();
	//if (typeid(*material) != typeid(Material_SolidColor)) {
	//	return;
	//}
	//
	//Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(material);
	//
	//Collider* otherCollider;
	//if (Physics::GetInstance()->Collided(this, otherCollider)) {
	//	mat->SetColor(glm::vec3(0, 1, 0));
	//}
	//else {
	//	mat->SetColor(glm::vec3(1, 0, 0));
	//}
}

//Setter and Getter
void BoxCollider::SetHalfSize(glm::vec3 halfSize) {
	this->halfSize = halfSize;
}

Bounds BoxCollider::GetBounds() {
	Transform* transform = this->gameObject->GetTransform();

	Bounds bounds;
	bounds.center = transform->position;
	bounds.halfSize = this->halfSize;

	bounds.axis[0] = transform->GetForward();
	bounds.axis[1] = transform->GetUp();
	bounds.axis[2] = transform->GetRight();

	return bounds;
}