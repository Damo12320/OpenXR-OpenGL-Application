#include "Material_SolidColor.h"

Material_SolidColor::Material_SolidColor() : Material() {
}

Material_SolidColor::Material_SolidColor(ShaderProgramm shader) : Material(shader) {
}

Material_SolidColor::Material_SolidColor(const char* vertexShader, const char* fragmentShader) : Material(vertexShader, fragmentShader) {
}

Material_SolidColor::~Material_SolidColor() {
}

void Material_SolidColor::Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model) {
	mesh.Bind();
	shader.Bind();

	shader.SetMatrix4x4("model", model);
	shader.SetMatrix4x4("view", renderInfo.ViewMatrix);
	shader.SetMatrix4x4("projection", renderInfo.ProjectionMatrix);
	shader.SetVec3("cameraPosition", renderInfo.CameraPosition);

	shader.SetVec3("color", this->color);
	this->shader.SetFloat("specular", this->specularStrength);

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

//Setters
void Material_SolidColor::SetSpecularStrength(float specularStrength) {
	this->specularStrength = specularStrength;
}
void Material_SolidColor::SetColor(glm::vec3 color) {
	this->color = color;
}