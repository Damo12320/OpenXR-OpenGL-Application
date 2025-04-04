#version 430 core
//!#define SOLUTION

uniform mat4 viewMatrix = mat4(1.0);

uniform sampler2D albedo;
uniform sampler2D normalMap;

uniform float specular = 0.1;
uniform vec2 textureScaling = vec2(1.0, 1.0);

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
	vec2 uv = textureScaling * i.texCoord;
	vec4 albedoColor = texture(albedo, uv);
	if (albedoColor.a <= 0.1) discard;

	gPosition = i.position;
	vec3 normal = texture(normalMap, uv).rgb;
	normal = normal * 2.0 - 1.0;
	gNormal = normalize(i.TBN * normal);

	gAlbedoSpec.rgb = albedoColor.rgb;
	gAlbedoSpec.a = specular;
}