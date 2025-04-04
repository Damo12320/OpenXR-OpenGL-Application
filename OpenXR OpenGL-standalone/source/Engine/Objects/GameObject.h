#pragma once

#include "Transform.h"

#include <vector>
#include <string>
#include <memory>
#include <typeinfo>

class Component;

class GameObject {
private:
	Transform transform;
	std::vector<std::unique_ptr<Component>> components;

	int id;
public:
	bool active = true;
public:
	GameObject();
	~GameObject();
	void Start();
	void Update(float deltaTime);

	Transform* GetTransform();
	template <class T> T* AddComponent();
	template <class T> T* GetComponent();


	bool operator == (const GameObject& c);

private:
	static int idCounter;
	static int GetNextId();

	////do not allow copying
	//GameObject(const GameObject& c) = delete;
	//GameObject& operator=(const GameObject& c) = delete;
};


template <class T> T* GameObject::AddComponent() {
	// Check if T is a derived from Component
	if (!std::is_base_of<Component, T>()) {
		return nullptr;
	}

	auto component = std::make_unique<T>(this);
	T* componentPtr = component.get();
	components.push_back(std::move(component));
	return componentPtr;
}

template <class T> T* GameObject::GetComponent() {
	// Check if T is a derived from Component
	if (!std::is_base_of<Component, T>()) {
		return nullptr;
	}

	for (int i = 0; i < components.size(); i++) {
		if (typeid(*components[i]) == typeid(T)) {
			return static_cast<T*>(components[i].get());
		}
	}
	return nullptr;
}