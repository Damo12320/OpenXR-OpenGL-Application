#include "SphereCollider.h"
#include "../../Objects/Components/MeshRenderer.h"
#include "../../Materials/Material_SolidColor.h"
#include "../Physics.h"

SphereCollider::SphereCollider(GameObject* gameObject) : Collider(gameObject) {
}

SphereCollider::~SphereCollider() {
	std::cout << "SphereCollider destroyed" << std::endl;
}

void SphereCollider::Start() {

}
void SphereCollider::Update(float deltaTime) {
	/*MeshRenderer* meshRenderer = this->gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer == nullptr) {
		return;
	}

	Material* material = meshRenderer->GetMaterial();
	if (typeid(*material) != typeid(Material_SolidColor)) {
		return;
	}

	Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(material);

	if (Physics::GetInstance()->SphereCollided(this)) {
		mat->SetColor(glm::vec3(0, 1, 0));
	}
	else {
		mat->SetColor(glm::vec3(0, 0, 1));
	}*/
}

//Setter and Getter
void SphereCollider::SetRadius(float radius) {
	this->radius = radius;
}
float SphereCollider::GetRadius() {
	return this->radius;
}

glm::vec3 SphereCollider::GetCenter() {
	return this->gameObject->GetTransform()->GetPosition();
}