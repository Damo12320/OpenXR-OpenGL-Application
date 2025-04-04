#pragma once

#include "../Gameobject.h"

class Component {
protected:
	GameObject* gameObject;
public:
	Component(GameObject* gameObject);
	virtual ~Component();

	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;

	GameObject* GetGameObject();
};