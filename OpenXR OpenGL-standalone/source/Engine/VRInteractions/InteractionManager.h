#pragma once

#include <map>
#include "../Scene.h"
#include "../Debug.h"

class VRInteractor;
class VRInteractable;

class InteractionManager {
private:
	static InteractionManager* instance;

	std::map<VRInteractor*, VRInteractable*> grabbing;


	InteractionManager() = default;
public:
	static InteractionManager* GetInstance();

	void RemoveInteractor(VRInteractor* interactor);
	void RemoveInteractable(VRInteractable* interactable);

	bool TryGrab(VRInteractor* interactor, VRInteractable* interactable);

	void ForceRemoveGrab(VRInteractor* interactor);
	void Throw(VRInteractor* interactor, glm::vec3 velocity);

	bool IsGrabbing(VRInteractor* interactor);
	bool IsGrabbed(VRInteractable* interactable);
	VRInteractor* GetCurrentInteractor(VRInteractable* interactable);

	VRInteractable* SphereCastInteractables(glm::vec3 center, float radius);

	bool GetNewPosition(VRInteractable* interactable, glm::vec3 &position);
	bool GetNewRotation(VRInteractable* interactable, glm::quat &rotation);

private:
	bool IsGrabbing(VRInteractor* interactor, VRInteractable* interactable);
};