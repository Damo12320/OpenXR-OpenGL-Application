#pragma once
#include "../../OpenGLComponents/Mesh/Mesh.h"
#include "../../View.h"
#include "../../OpenGLComponents/ShaderProgramm.h"

class Material {
protected:
	ShaderProgramm shader;
public:
	Material();
	Material(ShaderProgramm shader);
	Material(const char* vertexShader, const char* fragmentShader);
	virtual ~Material();

	virtual void Draw(const Mesh& mesh, const RenderInfo& renderInfo, const glm::mat4& model);

	// Setters
	virtual void SetShader(ShaderProgramm shader);
	virtual void SetShader(const char* vertexShader, const char* fragmentShader);

};