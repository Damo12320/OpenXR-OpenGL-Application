#include "Scene.h"

std::unique_ptr<Scene> Scene::currentScene = nullptr;

Scene::~Scene() {
	std::cout << "SCENE::Deleting" << std::endl;
}

void Scene::Start() {
	for (int i = 0; i < this->gameObjects.size(); i++) {
		this->gameObjects[i]->Start();
	}
}

void Scene::Update(float deltaTime) {
	for (int i = 0; i < this->gameObjects.size(); i++) {
		this->gameObjects[i]->Update(deltaTime);
	}
}

void Scene::Draw(const RenderInfo& renderInfo) {
	for (int i = 0; i < this->meshRenderers.size(); i++) {
		this->meshRenderers[i]->Draw(renderInfo);
	}
}

bool Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
	if (std::find(this->gameObjects.begin(), this->gameObjects.end(), gameObject) != this->gameObjects.end()) {
		return false;
	}
	GameObject* gameObjectPtr = gameObject.get();
	this->gameObjects.push_back(std::move(gameObject));

	MeshRenderer* meshRenderer = gameObjectPtr->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr) {
		this->meshRenderers.push_back(meshRenderer);
	}

	PointLight* pointLight = gameObjectPtr->GetComponent<PointLight>();
	if (pointLight != nullptr) {
		this->pointLights.push_back(pointLight);
	}

	SpotLight* spotLight = gameObjectPtr->GetComponent<SpotLight>();
	if (spotLight != nullptr) {
		this->spotLights.push_back(spotLight);
	}




	SphereCollider* sphereCollider = gameObjectPtr->GetComponent<SphereCollider>();
	if (sphereCollider != nullptr) {
		this->sphereColliders.push_back(sphereCollider);
	}

	PlaneCollider* planeCollider = gameObjectPtr->GetComponent<PlaneCollider>();
	if (planeCollider != nullptr) {
		this->planeColliders.push_back(planeCollider);
	}

	BoxCollider* boxCollider = gameObjectPtr->GetComponent<BoxCollider>();
	if (boxCollider != nullptr) {
		this->boxColliders.push_back(boxCollider);
	}




	Rigidbody* rigidbody = gameObjectPtr->GetComponent<Rigidbody>();
	if (rigidbody != nullptr) {
		this->rigidbodys.push_back(rigidbody);
	}

	return true;
}

void Scene::DestroyGameObject(GameObject* gameObject) {
	if (std::find(this->gameObjectsToDestroy.begin(), this->gameObjectsToDestroy.end(), gameObject) != this->gameObjectsToDestroy.end()) {
		return;
	}

	this->gameObjectsToDestroy.push_back(gameObject);
}

void Scene::DestroyGameObjects() {
	for (int i = 0; i < this->gameObjectsToDestroy.size(); i++) {
		GameObject* gameObject = this->gameObjectsToDestroy[i];

		//MeshRenderer
		for (int x = 0; x < this->meshRenderers.size(); x++) {
			if (this->meshRenderers[x]->GetGameObject() == gameObject) {
				this->meshRenderers.erase(this->meshRenderers.begin() + x);
				break;
			}
		}

		//pointLights
		for (int x = 0; x < this->pointLights.size(); x++) {
			if (this->pointLights[x]->GetGameObject() == gameObject) {
				this->pointLights.erase(this->pointLights.begin() + x);
				break;
			}
		}
		//spotLights
		for (int x = 0; x < this->spotLights.size(); x++) {
			if (this->spotLights[x]->GetGameObject() == gameObject) {
				this->spotLights.erase(this->spotLights.begin() + x);
				break;
			}
		}

		//sphereColliders
		for (int x = 0; x < this->sphereColliders.size(); x++) {
			if (this->sphereColliders[x]->GetGameObject() == gameObject) {
				this->sphereColliders.erase(this->sphereColliders.begin() + x);
				break;
			}
		}
		//planeColliders
		for (int x = 0; x < this->planeColliders.size(); x++) {
			if (this->planeColliders[x]->GetGameObject() == gameObject) {
				this->planeColliders.erase(this->planeColliders.begin() + x);
				break;
			}
		}
		//boxColliders
		for (int x = 0; x < this->boxColliders.size(); x++) {
			if (this->boxColliders[x]->GetGameObject() == gameObject) {
				this->boxColliders.erase(this->boxColliders.begin() + x);
				break;
			}
		}

		//rigidbodys
		for (int x = 0; x < this->rigidbodys.size(); x++) {
			if (this->rigidbodys[x]->GetGameObject() == gameObject) {
				this->rigidbodys.erase(this->rigidbodys.begin() + x);
				break;
			}
		}


		for (int x = 0; x < this->gameObjects.size(); x++) {
			if (this->gameObjects[x].get() == gameObject) {
				this->gameObjects[x].reset();
				this->gameObjects.erase(this->gameObjects.begin() + x);
			}
		}
	}
}


std::vector<GameObject*> Scene::SphereCastGameObjects(glm::vec3 center, float radius) {
	std::vector<GameObject*> objs;
	for (int i = 0; i < this->gameObjects.size(); i++) {
		if (glm::distance(this->gameObjects[i]->GetTransform()->GetPosition(), center) < radius) {
			objs.push_back(this->gameObjects[i].get());
		}
	}

	return objs;
}

//static
Scene* Scene::GetCurrentScene() {
	return currentScene.get();
}
void Scene::SetCurrentScene(std::unique_ptr<Scene> scene) {
	currentScene = std::move(scene);
}