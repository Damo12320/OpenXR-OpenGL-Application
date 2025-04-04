#pragma once

#include "Objects/Components/Component.h"

#include <algorithm>
#include "Objects/Components/MeshRenderer.h"
#include "Objects/Components/PointLight.h"
#include "Objects/Components/SpotLight.h"
#include "Physics/Components/SphereCollider.h"
#include "Physics/Components/PlaneCollider.h"
#include "Physics/Components/BoxCollider.h"
#include "Physics/Components/Rigidbody.h"
#include <memory>

class Scene {
private:
	static std::unique_ptr<Scene> currentScene;

	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<MeshRenderer*> meshRenderers;


	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;


	std::vector<SphereCollider*> sphereColliders;
	std::vector<PlaneCollider*> planeColliders;
	std::vector<BoxCollider*> boxColliders;
	std::vector<Rigidbody*> rigidbodys;

	std::vector<GameObject*> gameObjectsToDestroy;
public:
	~Scene();


	void Start();
	void Update(float deltaTime);

	void Draw(const RenderInfo& renderInfo);

	bool AddGameObject(std::unique_ptr<GameObject> gameObject);
	void DestroyGameObject(GameObject* gameObject);

	void DestroyGameObjects();

	std::vector<GameObject*> SphereCastGameObjects(glm::vec3 center, float radius);


	std::vector<PointLight*> *GetPointLights() { return &pointLights; }
	std::vector<SpotLight*> *GetSpotLights() { return &spotLights; }
	std::vector<SphereCollider*> *GetSphereColliders() { return &sphereColliders; }
	std::vector<PlaneCollider*> *GetPlaneColliders() { return &planeColliders; }
	std::vector<BoxCollider*> *GetBoxColliders() { return &boxColliders; }
	std::vector<Rigidbody*> *GetRigidbodys() { return &rigidbodys; }

	static Scene* GetCurrentScene();
	static void SetCurrentScene(std::unique_ptr<Scene> scene);
};