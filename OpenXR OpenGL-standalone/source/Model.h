#pragma once
#include "Engine/Scene.h"
#include "OpenGLComponents/Mesh/MeshImport.h"
#include "Engine/Objects/Components/VRController.h"
#include "Engine/VRInteractions/Components/VRInteractor.h"
#include "Engine/VRInteractions/Components/VRInteractable.h"
#include "Engine/Physics/Components/SphereCollider.h"
#include "Engine/Physics/Components/Rigidbody.h"

#include "Engine/Materials/MaterialFactory.h"

#include "GameSceneBuilder.h"

#include<map>

class Model {
public:
	void Start();
	void Update(float deltaTime);
};