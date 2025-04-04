#pragma once
#include "../Scene.h"
#include "Collisions.h"

class Physics {
private:

	struct Collision {
		Collider* firstCollider;
		Collider* secondCollider;
	};

	static Physics* instance;

	float gravity = -9.81;
	float floorheight = 0;

	std::vector<Collision> lastCollisions;

	Physics() = default;
public:

	static Physics* GetInstance();

	void Update(Scene* scene, float deltaTime);

	//returns the first collision in the vector. does not reflect distance to the collider
	bool Collided(Collider* col, Collider*& out_otherCollider);
	bool Collided(Collider* col, std::vector<Collider*>& out_otherColliders);

	float GetGravity() { return gravity; }

private:
	void CheckCollisionsSpheres(std::vector<SphereCollider*>* sphereColliders);
	void CheckCollisionsSpheresPlanes(std::vector<SphereCollider*>* sphereColliders, std::vector<PlaneCollider*>* planeColliders);
	void CheckCollisionsSpheresBoxes(std::vector<SphereCollider*>* sphereColliders, std::vector<BoxCollider*>* boxColliders);

	void CollisionResponseSphere2x(SphereCollider* col0, Rigidbody* rigid0, SphereCollider* col1, Rigidbody* rigid1);
	void CollisionResponseSpherePlane(SphereCollider* sphere, Rigidbody* rigid, PlaneCollider* plane);
	void CollisionResponseSphereBox(SphereCollider* sphere, Rigidbody* rigid, BoxCollider* box, glm::vec3 faceNormal);

	void AddCollision(Collision collision);
};