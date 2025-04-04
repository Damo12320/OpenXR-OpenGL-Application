#pragma once
#include "Engine/Scene.h"
#include "Engine/Materials/MaterialFactory.h"

#include "Engine/Objects/Components/VRController.h"
#include "Engine/Objects/Components/Projectile.h"
#include "Engine/Objects/Components/Breakable.h"
#include "Engine/Objects/Components/ProjectileHandSpawner.h"
#include "Engine/Objects/Components/ProjectileRespawner.h"
#include "Engine/VRInteractions/Components/VRInteractor.h"
#include "Engine/VRInteractions/Components/VRInteractable.h"

#include "OpenGLComponents/Mesh/MeshImport.h"

class GameSceneBuilder {
public:
	static void BuildArena(Scene* scene);
	static void AddSkybox(Scene* scene);
	static void AddTargets(Scene* scene);
	static void AddControllers(Scene* scene);
	static void AddBall(Scene* scene);

private:
	struct PlaneDef {
		glm::vec3 position;
		glm::vec3 eulerRotationDeg;

		glm::vec3 color;
	};
	struct TargetDef {
		glm::vec3 position;
		glm::vec3 eulerRotationDeg;
		glm::vec3 scale;

		glm::vec3 color;
	};


	static void BuildLCDPlane(Scene* scene, PlaneDef planeDefinition, const char* picturePath, bool respawnsBall = false);
	static void BuildPlane(Scene* scene, PlaneDef planeDefinition, bool respawnsBall = false);
	static void BuildTarget(Scene* scene, TargetDef targetDefinition);
};