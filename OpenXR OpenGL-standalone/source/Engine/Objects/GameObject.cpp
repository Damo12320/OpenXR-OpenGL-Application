#include "GameObject.h"
#include "Components/Component.h"

GameObject::GameObject() {
	this->id = GetNextId();
}

GameObject::~GameObject() {
	std::cout << "GAMEOBJECT::Deleting" << std::endl;
}

void GameObject::Start() {
	for (auto& component : components) {
		component->Start();
	}
}

void GameObject::Update(float deltaTime) {
	for (auto& component : components) {
		component->Update(deltaTime);
	}
}

Transform* GameObject::GetTransform() {
	return &transform;
}

bool GameObject::operator==(const GameObject& c) {
	return this->id == c.id;
}

//static
int GameObject::idCounter = 0;

int GameObject::GetNextId() {
	idCounter++;
	return idCounter;
}