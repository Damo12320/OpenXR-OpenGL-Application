#include "Material_Textures.h"

Material_Textures::Material_Textures() : Material() {
}

Material_Textures::Material_Textures(ShaderProgramm shader) : Material(shader) {

}

Material_Textures::Material_Textures(const char* vertexShader, const char* fragmentShader) : Material(vertexShader, fragmentShader) {

}

Material_Textures::~Material_Textures() {
	std::cout << "MATERIAL::TEXTURES::Deleting" << std::endl;
}

void Material_Textures::Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model) {
	mesh.Bind();
	this->shader.Bind();

	this->shader.SetMatrix4x4("model", model);
	this->shader.SetMatrix4x4("view", renderInfo.ViewMatrix);
	this->shader.SetMatrix4x4("projection", renderInfo.ProjectionMatrix);
	this->shader.SetVec3("cameraPosition", renderInfo.CameraPosition);

	//this->shader.SetFloat("specular", this->specularStrength);
	//this->shader.SetVec2("textureScaling", this->textureScaling);

	this->BindTextures();
	this->BindFloats();
	this->BindVec2s();
	this->BindVec3s();

	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void Material_Textures::BindTextures() {//TODO: add dynamic texture slots
	for (int i = 0; i < this->textures.size(); i++) {
		this->textures[i]->BindToUnit(0 + i);
		this->shader.SetTextureLocation(this->textureNames[i], 0 + i);
	}
}

void Material_Textures::BindFloats() {
	std::map<std::string, float>::iterator it = this->floatUniforms.begin();

	while (it != this->floatUniforms.end()) {
		this->shader.SetFloat(it->first, it->second);
		it++;
	}
}

void Material_Textures::BindVec2s() {
	std::map<std::string, glm::vec2>::iterator it = this->vec2Uniforms.begin();

	while (it != this->vec2Uniforms.end()) {
		this->shader.SetVec2(it->first, it->second);
		it++;
	}
}

void Material_Textures::BindVec3s() {
	std::map<std::string, glm::vec3>::iterator it = this->vec3Uniforms.begin();

	while (it != this->vec3Uniforms.end()) {
		this->shader.SetVec3(it->first, it->second);
		it++;
	}
}



//void Material_Textures::AddTexture(std::string name, GLTexture2D texture) {
//	this->textures.push_back(texture);
//	this->textureNames.push_back(name);
//}

void Material_Textures::AddTexture(std::string name, std::string texturePath) {
	GLTexture2D* tex = GLTexture2D::LoadTextureFromPath(texturePath);
	this->textures.push_back(tex);
	this->textureNames.push_back(name);
}

void Material_Textures::SetFloatUniform(std::string name, float value) {
	this->floatUniforms[name] = value;
}
void Material_Textures::SetVec2Uniform(std::string name, glm::vec2 value) {
	this->vec2Uniforms[name] = value;
}
void Material_Textures::SetVec3Uniform(std::string name, glm::vec3 value) {
	this->vec3Uniforms[name] = value;
}

void Material_Textures::RemoveTexture(std::string name) {
	for (int i = 0; i < this->textureNames.size(); i++) {
		if (this->textureNames[i] == name) {
			this->textures.erase(this->textures.begin() + i);
			this->textureNames.erase(this->textureNames.begin() + i);
			break;
		}
	}
}