#pragma once
#include "Scene.h"
#include "../OpenGLComponents/Mesh/MeshImport.h"
#include "Materials/MaterialFactory.h"

class Debug {
private:
	static Debug* instance;
	Debug() = default;

	static Debug* GetInstance();

	std::map<int, GameObject*> debugObjects;
	int latestID = 0;

	std::vector<GameObject*> printPositions;
public:
	static int CreateDebugSphere(glm::vec3 position = glm::vec3(0), glm::vec3 color = glm::vec3(1));
	static void SetPosition(int id, glm::vec3 position);
	static void SetColor(int id, glm::vec3 color);

	static void AddPrintPosition(GameObject* gameObject);

	static void Update();
};