#version 330 core

//!#define SOLUTION
out vec2 uv;


void main() 
{
	const vec2 vertices[4] = vec2[4](vec2(-1.0, -1.0),
		vec2( 1.0, -1.0),
		vec2(-1.0,  1.0),
		vec2( 1.0,  1.0));
	vec2 pos = vertices[gl_VertexID];
	uv = 0.5 * pos + 0.5;
	gl_Position = vec4(pos, 0.0, 1.0);
}