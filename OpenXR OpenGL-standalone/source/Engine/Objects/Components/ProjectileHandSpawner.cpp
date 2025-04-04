#include "ProjectileHandSpawner.h"

ProjectileHandSpawner::ProjectileHandSpawner(GameObject* gameObject) : Component(gameObject) {

}
ProjectileHandSpawner::~ProjectileHandSpawner() {

}

void ProjectileHandSpawner::Start() {

}
void ProjectileHandSpawner::Update(float deltaTime) {
	this->UpdateVelocity();

	if (this->spawnedBoulder != nullptr) {
		this->spawnedBoulder->GetTransform()->position = this->gameObject->GetTransform()->position;
		this->spawnedBoulder->GetTransform()->rotation = this->gameObject->GetTransform()->rotation;
	}

	bool grabButtonPressedNow = VRInput::GetInstance()->IsHandGrabbing(this->hand);
	if (this->grabButtonPressed == grabButtonPressedNow) return;

	if (this->grabButtonPressed == true && grabButtonPressedNow == false) {
		this->EndGrab();
	}
	else if (this->grabButtonPressed == false && grabButtonPressedNow == true) {
		this->BeginGrab();
	}

	this->grabButtonPressed = grabButtonPressedNow;
}

//Setter
void ProjectileHandSpawner::SetHand(Hand hand) {
	this->hand = hand;
}

//private
void ProjectileHandSpawner::BeginGrab() {
	//Spawn Boulder
	if (this->spawnedBoulder != nullptr) return;

	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>();
	Transform* transform = gameObject->GetTransform();
	transform->position = this->gameObject->GetTransform()->position;
	transform->scale = glm::vec3(0.1);

	MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
	Mesh mesh;
	if (!LoadMesh("assets/meshes/Primitives/smoothSphere.obj", mesh)) {
		std::cout << "ERROR::GAMESCENEBUILDER::PLANE::MESH_NOT_LOADED" << "\n";
	}
	meshRenderer->SetMesh(mesh);

	Material_SolidColor* mat = dynamic_cast<Material_SolidColor*>(MaterialFactory::GetInstance()->CreateMaterialInstance("simpleColor"));
	mat->SetColor(glm::vec3(0.0, 1.0, 0.0));
	meshRenderer->SetMaterial(mat);

	SphereCollider* coll = gameObject->AddComponent<SphereCollider>();
	coll->SetRadius(0.1);

	Rigidbody* rigidbody = gameObject->AddComponent<Rigidbody>();
	rigidbody->isKinematic = true;

	gameObject->AddComponent<Projectile>();

	this->spawnedBoulder = gameObject.get();

	Scene::GetCurrentScene()->AddGameObject(std::move(gameObject));
}
void ProjectileHandSpawner::EndGrab() {
	glm::vec3 velocity = this->GetVelocity();
	velocity *= 50.0;

	//Throw Boulder
	Rigidbody* body = this->spawnedBoulder->GetComponent<Rigidbody>();
	body->isKinematic = false;
	body->velocity *= 0;
	body->AddForce(velocity);

	//body->GetGameObject()->GetTransform()->position = this->GetGameObject()->GetTransform()->position;

	this->spawnedBoulder = nullptr;

}

glm::vec3 ProjectileHandSpawner::GetVelocity() {
	glm::vec3 velocity{};
	for (glm::vec3 value : this->velocitys) {
		velocity += value;
	}
	velocity /= (sizeof(this->velocitys) / sizeof(glm::vec3));
	return velocity;
}

void ProjectileHandSpawner::UpdateVelocity() {
	glm::vec3 velocity = this->gameObject->GetTransform()->GetPosition() - this->lastPosition;
	velocity = velocity / Time::DeltaTime();

	this->lastPosition = this->gameObject->GetTransform()->GetPosition();

	this->velocitys[this->velocityIndex] = velocity;
	this->velocityIndex++;
	if (this->velocityIndex >= sizeof(this->velocitys) / sizeof(glm::vec3)) {
		this->velocityIndex = 0;
	}
}