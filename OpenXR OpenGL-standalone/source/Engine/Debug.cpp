#include "Debug.h"

//private static
Debug* Debug::instance = nullptr;
//private static
Debug* Debug::GetInstance() {
	if (instance == nullptr) {
		instance = new Debug();
	}
	return instance;
}

int Debug::CreateDebugSphere(glm::vec3 position, glm::vec3 color){
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	gameObject->GetTransform()->position = position;
	gameObject->GetTransform()->scale = glm::vec3(0.03);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/smoothSphere.obj", mesh)) {
		std::cout << "ERROR::DEBUG::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	mat->SetColor(color);
	meshRenderer->SetMaterial(mat);

	Debug* data = GetInstance();
	data->debugObjects.insert(std::make_pair(data->latestID, gameObject.get()));
	data->latestID++;

	Scene::GetCurrentScene()->AddGameObject(std::move(gameObject));

	return data->latestID - 1;
}

void Debug::SetPosition(int id, glm::vec3 position) {
	Debug* data = GetInstance();

	if (data->debugObjects.find(id) != data->debugObjects.end()) {
		data->debugObjects[id]->GetTransform()->position = position;
	}
}

void Debug::SetColor(int id, glm::vec3 color) {
	Debug* data = GetInstance();

	if (data->debugObjects.find(id) != data->debugObjects.end()) {
		data->debugObjects[id]->GetTransform()->position = color;
	}
}

void Debug::AddPrintPosition(GameObject* gameObject) {
	Debug* data = GetInstance();

	data->printPositions.push_back(gameObject);
}

void Debug::Update() {
	Debug* data = GetInstance();

	for (int i = 0; i < data->printPositions.size(); i++) {
		glm::vec3 pos = data->printPositions[i]->GetTransform()->position;
		std::cout << "GameObject Pos" << " x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << std::endl;
	}
}