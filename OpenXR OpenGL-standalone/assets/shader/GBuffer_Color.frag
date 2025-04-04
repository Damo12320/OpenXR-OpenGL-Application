#version 430 core
//!#define SOLUTION

uniform mat4 viewMatrix = mat4(1.0);

uniform vec3 color;
uniform float specular = 0.1;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VertexData
{
	vec3 normal;
	vec3 position;
	vec2 texCoord;
	mat3 TBN;
} i;

void main() 
{
	gPosition = i.position;
	gNormal = normalize(i.normal);

	gAlbedoSpec.rgb = color;
	gAlbedoSpec.a = specular;
}