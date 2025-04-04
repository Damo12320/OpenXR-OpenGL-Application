#include "GameSceneBuilder.h"

void GameSceneBuilder::BuildArena(Scene* scene) {
	//Ground ------------------------------------------------------------------------
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = glm::vec3(0.0, 0.0, 0.0);
	transform->SetEulerRotationDeg(glm::vec3(0.0, 0.0, 0.0));
	transform->scale = glm::vec3(3.0, 3.0, 3.0);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/Plane.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);


	Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	mat->SetColor(glm::vec3(1.0, 1.0, 1.0));
	meshRenderer->SetMaterial(mat);

	PlaneCollider* coll = gameObject->AddComponent<PlaneCollider>();

	scene->AddGameObject(std::move(gameObject));
	//Ground end -------------------------------------------------------------------------
	
	
	//Blacksmith -------------------------------------------------------------------------
	/*gameObject = std::make_unique<GameObject>();
	transform = gameObject->GetTransform();
	transform->position = glm::vec3(3.0, 0.0, 0.0);
	transform->SetEulerRotationDeg(glm::vec3(0.0, 45.0, 0.0));

	meshRenderer = gameObject->AddComponent<MeshRenderer>();
	if (!LoadMesh("assets/meshes/PolygonModels/Buildings/BlackSmith.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	Material_Textures* matTex = dynamic_cast<Material_Textures*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleTextured"));
	matTex->AddTexture("albedo", "assets/textures/Polygon_tex.png");
	matTex->AddTexture("normalMap", "assets/textures/Polygon_Normals.png");
	meshRenderer->SetMaterial(matTex);

	scene->AddGameObject(std::move(gameObject));*/
	//Blacksmith end -------------------------------------------------------------------------

	gameObject = std::make_unique<GameObject>();
	transform = gameObject->GetTransform();
	transform->position = glm::vec3(1.0, 1.0, 0.0);
	transform->SetEulerRotationDeg(glm::vec3(0.0, 0.0, 0.0));
	transform->scale = glm::vec3(0.1, 0.1, 0.1);

	meshRenderer = gameObject->AddComponent<MeshRenderer>();
	if (!LoadMesh("assets/meshes/Primitives/smoothSphere.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	mat->SetColor(glm::vec3(0.1, 0.0, 0.0));
	meshRenderer->SetMaterial(mat);

	PointLight* light = gameObject->AddComponent<PointLight>();
	light->SetColor(glm::vec3(1.0, 0.0, 0.0));

	SphereCollider* collSphere = gameObject->AddComponent<SphereCollider>();
	collSphere->SetRadius(0.1);
	collSphere->elasticity = 2.0;

	Rigidbody* rigidbody = gameObject->AddComponent<Rigidbody>();
	rigidbody->isKinematic = true;

	VRInteractable* interactable = gameObject->AddComponent<VRInteractable>();
	interactable->HasGravity = false;
	scene->AddGameObject(std::move(gameObject));


	//PointLight* light = gameObject->AddComponent<PointLight>();
	//light->SetColor(glm::vec3(0.9, 0.6, 0.9) * 3.0f);
}

void GameSceneBuilder::AddSkybox(Scene* scene) {
	//std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	//Transform* transform = gameObject->GetTransform();
	//transform->position = glm::vec3(0, 20, 0);
	//transform->SetEulerRotationDeg(glm::vec3(0, 0, 0));
	//transform->scale = glm::vec3(0.1);
	//
	//MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	//Mesh mesh;
	//if (!LoadMesh("assets/meshes/PolygonModels/mountainSkybox.obj", mesh)) {
	//	std::cout << "ERROR::GAMESCENEBUILDER::MOUNTAIN_SKYBOX::MESH_NOT_LOADED" << "\n";
	//}
	//meshRenderer->SetMesh(mesh);
	//
	//Material_Textures* mat = dynamic_cast<Material_Textures*>(MaterialFactory::GetInstance()->CreateMaterialInstance("mountainSkybox"));
	//meshRenderer->SetMaterial(mat);
	//
	//scene->AddGameObject(std::move(gameObject));
}

void GameSceneBuilder::AddTargets(Scene* scene) {
	TargetDef targetDef;
	targetDef.position = glm::vec3(0.0, 1.0, 0.0);
	targetDef.eulerRotationDeg = glm::vec3(0.0, 0.0, 0.0);
	targetDef.scale = glm::vec3(0.075, 0.05, 0.1);
	targetDef.color = glm::vec3(1.0, 1.0, 0.0);

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 10; x++) {
			for (int z = 0; z < 8; z++) {
				targetDef.position = glm::vec3(-0.9, 0.06, -0.875) + (glm::vec3(x, y, z) * glm::vec3(0.2, 0.2, 0.25));
				BuildTarget(scene, targetDef);
			}
		}
	}
}

void GameSceneBuilder::AddControllers(Scene* scene) {
	MaterialFactory* materialFactory = MaterialFactory::GetInstance();

	//Left Hand
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	gameObject->GetTransform()->scale = glm::vec3(0.05);

	VRController* leftHand = gameObject->AddComponent<VRController>();
	leftHand->SetHand(Hand::LEFT);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/icoSphere.obj", mesh)) {
		std::cout << "ERROR::MAIN::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	meshRenderer->SetMaterial(materialFactory->CreateMaterialInstance("simpleColor"));

	VRInteractor* vrInteractor = gameObject->AddComponent<VRInteractor>();
	vrInteractor->SetHand(Hand::LEFT);
	
	SphereCollider* sphereCollider = gameObject->AddComponent<SphereCollider>();
	sphereCollider->SetRadius(0.05);

	//PointLight* light = gameObject->AddComponent<PointLight>();
	//light->SetColor(glm::vec3(0.9, 0.6, 0.9));

	//ProjectileHandSpawner* spawner = gameObject->AddComponent<ProjectileHandSpawner>();
	//spawner->SetHand(Hand::LEFT);

	scene->AddGameObject(std::move(gameObject));



	//Right Hand
	gameObject = std::make_unique<GameObject>();
	gameObject->GetTransform()->scale = glm::vec3(0.05);

	VRController* rightHand = gameObject->AddComponent<VRController>();
	rightHand->SetHand(Hand::RIGHT);

	meshRenderer = gameObject->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh(mesh);

	meshRenderer->SetMaterial(materialFactory->CreateMaterialInstance("simpleColor"));

	vrInteractor = gameObject->AddComponent<VRInteractor>();
	vrInteractor->SetHand(Hand::RIGHT);
	
	sphereCollider = gameObject->AddComponent<SphereCollider>();
	sphereCollider->SetRadius(0.05);

	//light = gameObject->AddComponent<PointLight>();
	//light->SetColor(glm::vec3(0.9, 0.9, 0.6));

	//spawner = gameObject->AddComponent<ProjectileHandSpawner>();
	//spawner->SetHand(Hand::RIGHT);

	scene->AddGameObject(std::move(gameObject));
}

void GameSceneBuilder::AddBall(Scene* scene) {
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = glm::vec3(0.0, 1.5, 0.0);
	transform->scale = glm::vec3(0.0025);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/LP_Basketball.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	Material_Textures* mat = dynamic_cast<Material_Textures*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleTextured"));
	mat->AddTexture("albedo", "assets/textures/Basketball_Base_Color.png");
	mat->AddTexture("normalMap", "assets/textures/Basketball_Normal_OpenGL.png");
	meshRenderer->SetMaterial(mat);

	SphereCollider* coll = gameObject->AddComponent<SphereCollider>();
	coll->SetRadius(0.1);
	coll->elasticity = 2.0;

	Rigidbody* rigidbody = gameObject->AddComponent<Rigidbody>();
	rigidbody->isKinematic = true;

	gameObject->AddComponent<VRInteractable>();
	gameObject->AddComponent<Projectile>();

	//PointLight* light = gameObject->AddComponent<PointLight>();
	//light->SetColor(glm::vec3(0.9, 0.6, 0.9) * 3.0f);

	scene->AddGameObject(std::move(gameObject));
}


//private
void GameSceneBuilder::BuildLCDPlane(Scene* scene, PlaneDef planeDefinition, const char* picturePath, bool respawnsBall) {
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = planeDefinition.position;
	transform->SetEulerRotationDeg(planeDefinition.eulerRotationDeg);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/Plane.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	Material_Textures* mat = dynamic_cast<Material_Textures*>(MaterialFactory::GetInstance()->CreateMaterialInstance("LCD"));
	mat->SetVec2Uniform("lcdResolution", glm::vec2(200, 200));
	mat->AddTexture("picture", picturePath);
	meshRenderer->SetMaterial(mat);

	PlaneCollider* coll = gameObject->AddComponent<PlaneCollider>();

	if (respawnsBall) {
		gameObject->AddComponent<ProjectileRespawner>();
	}

	scene->AddGameObject(std::move(gameObject));
}

void GameSceneBuilder::BuildPlane(Scene* scene, PlaneDef planeDefinition, bool respawnsBall) {
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = planeDefinition.position;
	transform->SetEulerRotationDeg(planeDefinition.eulerRotationDeg);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/Plane.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);


	Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	mat->SetColor(planeDefinition.color);
	meshRenderer->SetMaterial(mat);

	PlaneCollider* coll = gameObject->AddComponent<PlaneCollider>();

	if (respawnsBall) {
		gameObject->AddComponent<ProjectileRespawner>();
	}

	scene->AddGameObject(std::move(gameObject));
}

void GameSceneBuilder::BuildTarget(Scene* scene, TargetDef targetDefinition) {
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = targetDefinition.position;
	transform->SetEulerRotationDeg(targetDefinition.eulerRotationDeg);
	transform->scale = targetDefinition.scale;

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/cube.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::TARGET::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	//Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	//mat->SetColor(targetDefinition.color);
	//meshRenderer->SetMaterial(mat);

	Material_Textures* mat = dynamic_cast<Material_Textures*>(MaterialFactory::GetInstance()->CreateMaterialInstance("LCD"));
	mat->SetVec2Uniform("lcdResolution", glm::vec2(100, 100));
	mat->AddTexture("picture", "assets/textures/Skyboxes/MeadowCubemap/negy.jpg");
	meshRenderer->SetMaterial(mat);

	BoxCollider* coll = gameObject->AddComponent<BoxCollider>();
	coll->SetHalfSize(targetDefinition.scale);

	gameObject->AddComponent<Breakable>();

	scene->AddGameObject(std::move(gameObject));
}