#include "InteractionManager.h"
#include "Components/VRInteractor.h"
#include "Components/VRInteractable.h"

//static
InteractionManager* InteractionManager::instance = nullptr;
//static
InteractionManager* InteractionManager::GetInstance() {
	if (instance == nullptr) {
		instance = new InteractionManager();
	}
	return instance;
}


void InteractionManager::RemoveInteractor(VRInteractor* interactor) {
	this->ForceRemoveGrab(interactor);
}
void InteractionManager::RemoveInteractable(VRInteractable* interactable) {
	VRInteractor* interactor = this->GetCurrentInteractor(interactable);
	if (interactor != nullptr) {
		this->ForceRemoveGrab(interactor);
	}
}



bool InteractionManager::TryGrab(VRInteractor* interactor, VRInteractable* interactable) {
	if (interactor == nullptr || interactable == nullptr);
	if (!interactor->CanGrab() || !interactable->CanBeGrabbed()) return false;
	if (this->IsGrabbing(interactor, interactable)) return true;
	if (this->IsGrabbing(interactor)) return false;

	this->grabbing[interactor] = interactable;

	std::cout << "garbbingInteractionCount: " << this->grabbing.size() << std::endl;

	interactor->OnGrab();
	interactable->OnGrab();

	Rigidbody* body = this->grabbing[interactor]->GetGameObject()->GetComponent<Rigidbody>();
	body->velocity *= 0;
	body->isKinematic = true;

	Debug::AddPrintPosition(interactable->GetGameObject());

	return true;
}


void InteractionManager::ForceRemoveGrab(VRInteractor* interactor) {
	if (!this->IsGrabbing(interactor)) return;

	this->grabbing[interactor]->OnGrabRemove();
	interactor->OnGrabRemove();

	this->grabbing.erase(interactor);

	std::cout << "garbbingInteractionCount: " << this->grabbing.size() << std::endl;
}

void InteractionManager::Throw(VRInteractor* interactor, glm::vec3 velocity) {
	if (!this->IsGrabbing(interactor)) return;

	this->grabbing[interactor]->OnGrabRemove();
	interactor->OnGrabRemove();

	if (this->grabbing[interactor]->HasGravity) {
		Rigidbody* body = this->grabbing[interactor]->GetGameObject()->GetComponent<Rigidbody>();
		body->isKinematic = false;
		body->velocity *= 0;
		body->AddForce(velocity);
	}

	this->grabbing[interactor]->GetGameObject()->GetTransform()->position = interactor->GetGameObject()->GetTransform()->position;

	this->grabbing.erase(interactor);
}


bool InteractionManager::IsGrabbing(VRInteractor* interactor) {
	return this->grabbing.find(interactor) != this->grabbing.end();
}
bool InteractionManager::IsGrabbed(VRInteractable* interactable) {
	std::map<VRInteractor*, VRInteractable*>::iterator it = this->grabbing.begin();

	while (it != this->grabbing.end()) {
		if (it->second == interactable) {
			return true;
		}
		it++;
	}

	return false;
}
VRInteractor* InteractionManager::GetCurrentInteractor(VRInteractable* interactable) {
	std::map<VRInteractor*, VRInteractable*>::iterator it = this->grabbing.begin();

	while (it != this->grabbing.end()) {
		if (it->second == interactable) {
			return it->first;
		}
		it++;
	}

	return nullptr;
}

VRInteractable* InteractionManager::SphereCastInteractables(glm::vec3 center, float radius) {
	std::vector<GameObject*> objs = Scene::GetCurrentScene()->SphereCastGameObjects(center, radius);

	for (int i = 0; i < objs.size(); i++) {
		VRInteractable* interatable = objs[i]->GetComponent<VRInteractable>();
		if (interatable != nullptr) {
			return interatable;
		}
	}

	return nullptr;
}

bool InteractionManager::GetNewPosition(VRInteractable* interactable, glm::vec3 &position) {
	VRInteractor* interactor = this->GetCurrentInteractor(interactable);
	if (interactor != nullptr) {
		position = interactor->GetGameObject()->GetTransform()->position;
		return true;
	}

	return false;
}

bool InteractionManager::GetNewRotation(VRInteractable* interactable, glm::quat &rotation) {
	VRInteractor* interactor = this->GetCurrentInteractor(interactable);
	if (interactor != nullptr) {
		rotation = interactor->GetGameObject()->GetTransform()->rotation;
		return true;
	}

	return false;
}

//Private
bool InteractionManager::IsGrabbing(VRInteractor* interactor, VRInteractable* interactable) {
	if (this->grabbing.find(interactor) != this->grabbing.end()) {
		if (this->grabbing[interactor] == interactable) return true;
	}

	return false;
}