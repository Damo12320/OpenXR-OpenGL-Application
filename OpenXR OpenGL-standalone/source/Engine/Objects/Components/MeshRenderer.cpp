#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(GameObject* gameObject) : Component(gameObject) {
}

MeshRenderer::~MeshRenderer() {
	std::cout << "MESHRENDERER::Deleting" << std::endl;
}

void MeshRenderer::Start() {

}

void MeshRenderer::Update(float deltaTime) {

}

void MeshRenderer::Draw(const RenderInfo& renderInfo) {
	if (!this->gameObject->active) return;

	glm::mat4 model{};
	this->gameObject->GetTransform()->GetModelMatrix(model);

	material->Draw(this->mesh, renderInfo, model);
}


// Setters
void MeshRenderer::SetMesh(Mesh mesh) {
	this->mesh = mesh;
}
void MeshRenderer::SetMaterial(Material* material) {
	this->material = material;
}

Material* MeshRenderer::GetMaterial() {
	return this->material;
}