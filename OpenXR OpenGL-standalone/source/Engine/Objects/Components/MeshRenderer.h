#pragma once

#include "Component.h"
#include "../../../OpenGLComponents/Mesh/Mesh.h"
#include "../../Materials/Material.h"
#include "../../../View.h"

class MeshRenderer : public Component {
private:
	Mesh mesh;
	Material* material = nullptr;

public:
	MeshRenderer(GameObject* gameObject);
	~MeshRenderer();

	void Start() override;
	void Update(float deltaTime) override;

	void Draw(const RenderInfo& renderInfo);

	// Setters
	void SetMesh(Mesh mesh);
	void SetMaterial(Material* material);

	Material* GetMaterial();
};