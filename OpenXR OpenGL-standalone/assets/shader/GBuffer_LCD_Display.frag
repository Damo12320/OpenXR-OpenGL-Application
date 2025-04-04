#version 430 core
//!#define SOLUTION

uniform mat4 viewMatrix = mat4(1.0);

uniform vec3 cameraPosition;

uniform sampler2D picture;
uniform float specular = 0.1;
uniform vec2 lcdResolution = vec2(100.0, 100.0);

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

//more efficient shader
//https://www.shadertoy.com/view/3sGXWc

float rectangle(vec2 uv, vec2 center, vec2 dimensions){
    vec2 bottomLeftCorner = center - dimensions / 2.0;
    vec2 topRightCorner = center + dimensions / 2.0;
    
    
    vec2 bottomLeft = step(bottomLeftCorner, uv);
    vec2 topRight = 1.0 - step(topRightCorner, uv);
    
    float rectangle = bottomLeft.x * bottomLeft.y;
    rectangle *= topRight.x * topRight.y;
    
    return rectangle;
}

void main() 
{
	gPosition = i.position;
	gNormal = normalize(i.normal);

	gAlbedoSpec.a = specular;

    //LCD Effect
    vec2 st = i.texCoord * lcdResolution;
    vec2 pictureUV = st;

     st = fract(st);
    
    vec3 color = vec3(0);

    //draw illumination cells
    vec3 rec = vec3(0);
    rec = vec3(1, 0, 0) * rectangle(st, vec2(0.3333 * 0.5, 0.5), vec2(0.2, 0.9));
    color = rec;
    
    rec = vec3(0, 1, 0) * rectangle(st, vec2(0.3333 * 0.5 + 0.333, 0.5), vec2(0.2, 0.9));
    color = mix(color, rec, 0.5);
    
    rec = vec3(0, 0, 1) * rectangle(st, vec2(0.3333 * 0.5 + 0.666, 0.5), vec2(0.2, 0.9));
    color = mix(color, rec, 0.5);
    
    color *= 5.0;

    vec2 test = floor(pictureUV) / lcdResolution;
    vec3 pictureColorPixelated = texture(picture, test).rgb;

    vec3 finalColor = color * pictureColorPixelated;

    vec3 pictureColor = texture(picture, i.texCoord).rgb;

    float distanceToCamera = length(i.position - cameraPosition);

    finalColor = mix(finalColor, pictureColor, smoothstep(0.1, 1.2, distanceToCamera));

    gAlbedoSpec.rgb = finalColor;
}