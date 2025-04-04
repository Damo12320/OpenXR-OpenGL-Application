#include "VRController.h"
#include "MeshRenderer.h"

VRController::VRController(GameObject* gameObject) : Component(gameObject) {
}

VRController::~VRController() {
	std::cout << "VRController destroyed of " << this->GetHandString() << std::endl;
}

void VRController::Start() {
	MeshRenderer* meshRenderer = this->gameObject->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr) {
		Material* material = meshRenderer->GetMaterial();
		if (typeid(*material) == typeid(Material_SolidColor)) {
			this->controllerMaterial = dynamic_cast<Material_SolidColor*>(material);
		}
	}
}

void VRController::Update(float deltaTime) {
	VRInput* vrInput = VRInput::GetInstance();
	VRInputTransform transform = vrInput->GetHandTransform(this->hand);

	this->gameObject->GetTransform()->position = transform.position;
	this->gameObject->GetTransform()->rotation = transform.rotation;

	glm::vec3 color = vrInput->GetHandGrabValue(this->hand) * glm::vec3(0.0f, 1.0f, 0.0f);
	color += vrInput->GetHandSelectValue(this->hand) * glm::vec3(1.0f, 0.0f, 0.0f);
	color = glm::clamp(color, 0.0f, 1.0f);

	if (color == glm::vec3(0)) {
		color = glm::vec3(1.0f);
	}
	this->controllerMaterial->SetColor(color);
}

void VRController::SetHand(Hand hand) {
	this->hand = hand;
}

std::string VRController::GetHandString() {
	return hand == Hand::LEFT ? "left hand" : "right hand";
}