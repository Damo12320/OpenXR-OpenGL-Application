#pragma once

#include "Components/SphereCollider.h"
#include "Components/PlaneCollider.h"
#include "Components/BoxCollider.h"
#include <glm/gtx/vector_angle.hpp>

#include "../Debug.h"

class Collisions {
private:
	static int debugid;
public:
	static bool SphereSphere(SphereCollider* col0, SphereCollider* col1, glm::vec3& penetration);
	static bool SphereFloor(SphereCollider* col0, float floorheight, float &penetration);
	static bool SpherePlane(SphereCollider* col0, PlaneCollider* planeCol, glm::vec3& penetration);
	static bool SphereBox(SphereCollider* col0, BoxCollider* boxCol, glm::vec3& penetration);
};