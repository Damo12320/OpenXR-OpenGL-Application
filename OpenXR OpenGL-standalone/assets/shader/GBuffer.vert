#version 430 core

//!#define SOLUTION
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

layout (location = 0) in  vec3 position;
layout (location = 1) in  vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VertexData
{
	vec3 normal;
	vec3 position;
	vec2 texCoord;
	mat3 TBN;
} o;

void WorldSpace()
{
	o.position = (model * vec4(position, 1.0)).xyz;

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	o.TBN = mat3(T, B, N);

	o.normal =  normalize((transpose(inverse(model)) * vec4(normal, 0)).xyz);
}

//void ViewSpace()
//{
//	o.position = (view * model * vec4(position, 1.0)).xyz;
//
//	vec3 T = normalize(vec3(view * model * vec4(tangent,   0.0)));
//	vec3 B = normalize(vec3(view * model * vec4(bitangent, 0.0)));
//	vec3 N = normalize(vec3(view * model * vec4(normal,    0.0)));
//	o.TBN = mat3(T, B, N);
//
//	o.normal =  normalize((transpose(inverse(view * model)) * vec4(normal, 0))).xyz;
//}

void main() 
{
	WorldSpace();

	//o.position = (model * vec4(position, 1.0)).xyz;

	//o.normal = normal;

	o.texCoord = texCoord;
	gl_Position = projection * view * model * vec4(position, 1);
}