#include "Physics.h"

//private static
Physics* Physics::instance = nullptr;
//private static
Physics* Physics::GetInstance() {
	if (instance == nullptr) {
		instance = new Physics();
	}
	return instance;
}

//public
void Physics::Update(Scene* scene, float deltaTime) {
	std::vector<Rigidbody*>* rigidbodys = scene->GetRigidbodys();
	for (int i = 0; i < rigidbodys->size(); i++) {
		rigidbodys->at(i)->UpdatePhysics(deltaTime);
	}


	this->lastCollisions.clear();
	this->CheckCollisionsSpheres(scene->GetSphereColliders());
	this->CheckCollisionsSpheresPlanes(scene->GetSphereColliders(), scene->GetPlaneColliders());
	this->CheckCollisionsSpheresBoxes(scene->GetSphereColliders(), scene->GetBoxColliders());
}

//public
bool Physics::Collided(Collider* col, Collider*& out_otherCollider) {
	for (int i = 0; i < this->lastCollisions.size(); i++) {
		if (this->lastCollisions[i].firstCollider == col ) {
			out_otherCollider = this->lastCollisions[i].secondCollider;
			return true;
		}
		else if (this->lastCollisions[i].secondCollider == col) {
			out_otherCollider = this->lastCollisions[i].firstCollider;
			return true;
		}
	}
	out_otherCollider = nullptr;
	return false;
}

bool Physics::Collided(Collider* col, std::vector<Collider*>& out_otherColliders) {
	if (out_otherColliders.size() > 0) {
		std::cout << "WARNING::PHYSICS::COLLIDED::Array to store Colliders is not empty!" << std::endl;
		return false;
	}

	for (int i = 0; i < this->lastCollisions.size(); i++) {
		if (this->lastCollisions[i].firstCollider == col) {
			out_otherColliders.push_back(this->lastCollisions[i].secondCollider);
		}
		else if (this->lastCollisions[i].secondCollider == col) {
			out_otherColliders.push_back(this->lastCollisions[i].firstCollider);
		}
	}

	return out_otherColliders.size() > 0;
}

//private
void Physics::CheckCollisionsSpheres(std::vector<SphereCollider*>* sphereColliders) {
	for (int i = 0; i < sphereColliders->size(); i++) {
		SphereCollider* col0 = sphereColliders->at(i);

		if (!col0->GetGameObject()->active) continue;

		for (int j = i + 1; j < sphereColliders->size(); j++) {
			SphereCollider* col1 = sphereColliders->at(j);

			if (col0 == col1 || !col1->GetGameObject()->active) {
				continue;
			}

			glm::vec3 penetration = glm::vec3(0);
			if (Collisions::SphereSphere(col0, col1, penetration)) {
				struct Collision collision = { col0 , col1 };
				this->AddCollision(collision);

				if (!col0->IsTrigger && !col1->IsTrigger) {
					Rigidbody* rigidbody0 = col0->GetGameObject()->GetComponent<Rigidbody>();
					Rigidbody* rigidbody1 = col1->GetGameObject()->GetComponent<Rigidbody>();
					if (rigidbody0 != nullptr) {
						//rigidbody0->velocity = glm::vec3(0);

						rigidbody0->GetGameObject()->GetTransform()->position += penetration;
					}

					if (rigidbody1 != nullptr) {
						//rigidbody1->velocity = glm::vec3(0);

						rigidbody1->GetGameObject()->GetTransform()->position -= penetration;
					}

					this->CollisionResponseSphere2x(col0, rigidbody0, col1, rigidbody1);
				}
			}
		}

		//float penetration = 0;
		//if (Collisions::SphereFloor(col0, this->floorheight, penetration)) {
		//	Rigidbody* rigidbody = col0->GetGameObject()->GetComponent<Rigidbody>();
		//	if (rigidbody != nullptr) {
		//		//rigidbody->velocity = glm::vec3(0);
		//		rigidbody->GetGameObject()->GetTransform()->position.y += penetration;
		//		this->CollisionResponseFloor(col0, rigidbody);
		//	}
		//}
	}
}

void Physics::CheckCollisionsSpheresPlanes(std::vector<SphereCollider*>* sphereColliders, std::vector<PlaneCollider*>* planeColliders) {
	if (sphereColliders->size() == 0 || planeColliders->size() == 0) {
		return;
	}


	for (int i = 0; i < sphereColliders->size(); i++) {
		SphereCollider* sphere = sphereColliders->at(i);

		if (!sphere->GetGameObject()->active) continue;

		for (int j = 0; j < planeColliders->size(); j++) {
			PlaneCollider* plane = planeColliders->at(j);

			if (!plane->GetGameObject()->active) continue;

			glm::vec3 penetration = glm::vec3(0);
			if (Collisions::SpherePlane(sphere, plane, penetration)) {
				struct Collision collision = { sphere , plane };
				this->AddCollision(collision);

				if (!sphere->IsTrigger && !plane->IsTrigger) {
					Rigidbody* rigidbody = sphere->GetGameObject()->GetComponent<Rigidbody>();
					if (rigidbody != nullptr) {
						rigidbody->GetGameObject()->GetTransform()->position += penetration;
					}

					CollisionResponseSpherePlane(sphere, rigidbody, plane);
				}
			}
		}
	}
}

void Physics::CheckCollisionsSpheresBoxes(std::vector<SphereCollider*>* sphereColliders, std::vector<BoxCollider*>* boxColliders) {
	if (sphereColliders->size() == 0 || boxColliders->size() == 0) {
		return;
	}

	for (int i = 0; i < sphereColliders->size(); i++) {
		SphereCollider* sphere = sphereColliders->at(i);

		if (!sphere->GetGameObject()->active) continue;

		for (int j = 0; j < boxColliders->size(); j++) {
			BoxCollider* box = boxColliders->at(j);

			if (!box->GetGameObject()->active) continue;

			glm::vec3 penetration = glm::vec3(0);
			if (Collisions::SphereBox(sphere, box, penetration)) {
				struct Collision collision = { sphere , box };
				this->AddCollision(collision);

				if (!sphere->IsTrigger && !box->IsTrigger) {
					Rigidbody* rigidbody = sphere->GetGameObject()->GetComponent<Rigidbody>();
					if (rigidbody != nullptr) {
						rigidbody->GetGameObject()->GetTransform()->position += penetration;
					}
				
					CollisionResponseSphereBox(sphere, rigidbody, box, glm::normalize(penetration));
				}
			}
		}
	}
}


void Physics::CollisionResponseSphere2x(SphereCollider* col0, Rigidbody* rigid0, SphereCollider* col1, Rigidbody* rigid1) {
	glm::vec3 velocity0, velocity1;
	float invertedMass0 = 0;
	float invertedMass1 = 0;

	bool anyRigidbodys = false;

	if (rigid0 != nullptr) {
		velocity0 = rigid0->velocity;
		invertedMass0 = rigid0->invertedMass;
		anyRigidbodys = true;
	}

	if (rigid1 != nullptr) {
		velocity1 = rigid1->velocity;
		invertedMass1 = rigid1->invertedMass;
		anyRigidbodys = true;
	}

	if (!anyRigidbodys) return;

	glm::vec3 normal = col0->GetCenter() - col1->GetCenter();
	normal = glm::normalize(normal);
	glm::vec3 relativVelocity = velocity0 - velocity1;

	float seperatingVelocity = glm::dot(relativVelocity, normal);
	float newSeperatingVelocity = -seperatingVelocity * std::min(col0->elasticity, col1->elasticity);

	float seperatingVelocityDiff = newSeperatingVelocity - seperatingVelocity;
	float impulse = seperatingVelocityDiff / (invertedMass0 + invertedMass1);
	glm::vec3 impulseVector = normal * impulse;


	if (rigid0 != nullptr) {
		//rigid0->AddForce(impulseVector);
		rigid0->velocity += impulseVector * rigid0->invertedMass;
	}

	if (rigid1 != nullptr) {
		//rigid1->AddForce(-impulseVector);
		rigid1->velocity += impulseVector * -rigid1->invertedMass;
	}
}

void Physics::CollisionResponseSpherePlane(SphereCollider* sphere, Rigidbody* rigid, PlaneCollider* plane) {
	glm::vec3 newVel = glm::reflect(rigid->velocity, plane->GetNormal());
	newVel *= std::min(sphere->elasticity, plane->elasticity);

	rigid->velocity = newVel;
}

void Physics::CollisionResponseSphereBox(SphereCollider* sphere, Rigidbody* rigid, BoxCollider* box, glm::vec3 faceNormal) {
	glm::vec3 newVel = glm::reflect(rigid->velocity, faceNormal);
	newVel *= std::min(sphere->elasticity, box->elasticity);

	rigid->velocity = newVel;
}



//private
void Physics::AddCollision(Collision collision) {
	for (int i = 0; i < this->lastCollisions.size(); i++) {
		if (this->lastCollisions[i].firstCollider == collision.firstCollider &&
			this->lastCollisions[i].secondCollider == collision.secondCollider) {
			return;
		}
	}

	this->lastCollisions.push_back(collision);
}