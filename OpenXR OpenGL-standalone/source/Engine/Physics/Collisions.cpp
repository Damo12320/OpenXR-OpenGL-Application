#include "Collisions.h"

int Collisions::debugid = -1;

//public static
bool Collisions::SphereSphere(SphereCollider* col0, SphereCollider* col1, glm::vec3& penetration) {
	float radius = col0->GetRadius() + col1->GetRadius();
	float distance = glm::distance(col0->GetCenter(), col1->GetCenter());

	glm::vec3 diff = col0->GetCenter() - col1->GetCenter();
	diff = glm::normalize(diff);

	float penetrationDistance = radius - distance;

	penetration = diff * penetrationDistance;
	penetration *= 0.5;

	return distance <= radius;
}
//public static
bool Collisions::SphereFloor(SphereCollider* col, float floorheight, float &penetration) {
	float bottomHeight = col->GetCenter().y - col->GetRadius();

	penetration = floorheight - bottomHeight;

	return bottomHeight <= floorheight;
}
//public static
bool Collisions::SpherePlane(SphereCollider* col0, PlaneCollider* planeCol, glm::vec3& penetration) {
	glm::vec3 normal = planeCol->GetNormal();
	glm::vec3 h = col0->GetCenter() - planeCol->GetOrigin();
	//glm::vec3 h = planeCol->GetOrigin() - col0->GetCenter();

	float distance = glm::dot(normal, h);

	//if (distance < 0) return false;

	penetration = normal * (col0->GetRadius() - distance);

	return distance <= col0->GetRadius();
}
//public static
//https://www.youtube.com/watch?v=cXryhth2Tzo
bool Collisions::SphereBox(SphereCollider* col0, BoxCollider* boxCol, glm::vec3& penetration) {
	Bounds box = boxCol->GetBounds();
	glm::quat boxRotation = boxCol->GetGameObject()->GetTransform()->rotation;

	//everything in localspace of the box
	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = box.halfSize * 2.0f;

	glm::vec3 r = col0->GetCenter() - box.center;
	r = r * boxRotation;
	glm::vec3 localCircleCenter = r + box.halfSize;

	//glm::vec3 closestPoint = glm::clamp(localCircleCenter, min, max);

	float x = std::max(min.x, std::min(localCircleCenter.x, max.x));
	float y = std::max(min.y, std::min(localCircleCenter.y, max.y));
	float z = std::max(min.z, std::min(localCircleCenter.z, max.z));
	glm::vec3 closestPoint = glm::vec3(x, y, z);


	//everything in worldspace
	glm::vec3 closestPointWorld = closestPoint - box.halfSize;
	closestPointWorld = boxRotation * closestPointWorld;
	closestPointWorld = closestPointWorld + box.center;

	glm::vec3 boxToCircle = col0->GetCenter() - closestPointWorld;

	float distance = glm::length(boxToCircle);


	glm::vec3 relativeCenter = localCircleCenter - box.halfSize;//box center in box localposition
	glm::vec3 q = glm::abs(relativeCenter) - box.halfSize;

	float t = std::max(q.x, std::max(q.y, q.z));
	float signedDistance = glm::length(glm::max(q, glm::vec3(0.0))) + std::min(t, 0.0f);


	penetration = glm::normalize(boxToCircle) * (col0->GetRadius() - distance);

	//Debug
	//if (debugid == -1) {
	//	debugid = Debug::CreateDebugSphere();
	//}
	//if (signedDistance <= col0->GetRadius()) {
	//	Debug::SetPosition(debugid, closestPointWorld);
	//}

	return signedDistance <= col0->GetRadius();
}