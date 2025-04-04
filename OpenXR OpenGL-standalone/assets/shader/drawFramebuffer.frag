#version 330 core
//!#define SOLUTION
uniform sampler2D frameBufferImage;
uniform float exposure = 1;

in vec2 uv;

out vec4 color;

vec3 tonemapping(vec3 color)
{
	return vec3(1)- exp(-color * exposure);
}

// All components are in the range [0…1], including hue.
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main() 
{
	vec3 finalColor = texture(frameBufferImage, uv).rgb;

	finalColor = tonemapping(finalColor);
	//finalColor = vec3(1);

	//finalColor += vec3(0, 1, 0);

    vec3 hsv = rgb2hsv(finalColor);
    hsv.y *= 1.2;

	color = vec4(hsv2rgb(hsv), 1);
}