#include "Material.h"

Material::Material() {
}

Material::Material(ShaderProgramm shader){
	this->shader = shader;
}

Material::Material(const char* vertexShader, const char* fragmentShader) {
	this->shader = ShaderProgramm(vertexShader, fragmentShader);
}

Material::~Material() {
	std::cout << "MATERIAL::Deleting" << std::endl;
}

void Material::Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model) {
	mesh.Bind();
	shader.Bind();

	shader.SetMatrix4x4("model", model);
	shader.SetMatrix4x4("view", renderInfo.ViewMatrix);
	shader.SetMatrix4x4("projection", renderInfo.ProjectionMatrix);
	shader.SetVec3("cameraPosition", renderInfo.CameraPosition);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

// Setters
void Material::SetShader(ShaderProgramm shader) {
	this->shader = shader;
}

void Material::SetShader(const char* vertexShader, const char* fragmentShader) {
	this->shader = ShaderProgramm(vertexShader, fragmentShader);
}