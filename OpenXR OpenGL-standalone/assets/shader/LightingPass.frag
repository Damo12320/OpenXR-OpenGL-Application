#version 330 core
//!#define SOLUTION

struct DirectionalLight{
	vec3 color;
	
	mat4 lightViewProjection;
	vec3 direction;
	float specularStrength;
};

struct PointLight{
	vec3 color;

	float constant;
    float linear;
    float quadratic;
	
	vec3 position;
	float specularStrength;
};

struct SpotLight{
	vec3 color;
	
	vec3 position;
	vec3 direction;
	float openingAngle;

	float constant;
    float linear;
    float quadratic;
	float specularStrength;
};

uniform vec3 ambientLightColor;
const int NR_LIGHTS = 32;

uniform DirectionalLight sun;
uniform PointLight pointLights[NR_LIGHTS];
uniform SpotLight spotLights[NR_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 cameraPosition;

in vec2 uv;

out vec4 color;

//-------------------LIHTING-BASICS----------------------------------------------------------------------------------
float lambert(vec3 normal, vec3 lightVec)
{
	return max(0, dot(normal, lightVec));
}

vec3 ambient(vec3 ambient, vec3 material)
{
	return ambient * material;
}

vec3 diffuse(vec3 normal, vec3 lightVec, vec3 lightcolor, vec3 materialcolor)
{
	return lambert(normal, lightVec) * lightcolor * materialcolor;
}

float specular(vec3 lightDir, vec3 normal, vec3 viewDirection, float specularStrength)
{
	vec3 reflection = reflect(-lightDir, normal);
	float direction = max(0, dot(viewDirection, reflection));

	return pow(direction, specularStrength);
}

float blinnSpecular(vec3 lightDir, vec3 normal, vec3 viewDirection, float specularStrength)
{
	vec3 halfwayDir = normalize(lightDir + viewDirection);  

	float direction = max(0, dot(normal, halfwayDir));

	return pow(direction, specularStrength);
}
//-----------------------------------------------------------------------------------------------------------

vec3 directionalLight(vec3 materialColor, vec3 normal, vec3 lightDirection, vec3 lightColor, float specularity)
{
	float specular = blinnSpecular(-lightDirection, normal, normalize(cameraPosition), 64);

	return diffuse(normal, -lightDirection, lightColor, materialColor) + specular * lightColor * specularity;
	//return diffuse(normal, -lightDirection, lightColor, materialColor);
}

vec3 pointLight(vec3 position, vec3 materialColor, vec3 normal, float specularity, vec3 viewDirection, int pointLightIndex)
{
	PointLight light = pointLights[pointLightIndex];
	vec3 toLight = normalize(light.position - position);

	//TODO: Shininess (64) in Material
	//float specular = specular(toLight, normal, viewDirection, 64);
	float specular = blinnSpecular(toLight, normal, viewDirection, 64);

	return diffuse(normal, toLight, light.color, materialColor) + specular * light.color * specularity;
}

vec3 pointLightWithAttenuation(vec3 position, vec3 materialColor, vec3 normal, float specularity, vec3 viewDirection, PointLight light)
{
    // attenuation
    float dist = length(light.position - position);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    //float attenuation = 1.0 / (pointLights[0].constant + pointLights[0].linear * dist + pointLights[pointLightIndex].quadratic * (dist * dist));
	//float attenuation = 1.0 / (1 + 0.09 * dist + 0.032 * (dist * dist));
	
	vec3 toLight = normalize(light.position - position);
	
	//Diffuse
	vec3 diffuse = diffuse(normal, toLight, light.color, materialColor);
	diffuse *= attenuation;
	
	//Specular
    float specular = blinnSpecular(toLight, normal, viewDirection, 64);
	specular *= attenuation;
	vec3 specularColor = specular * light.color * specularity;
	
    return diffuse + specularColor;
}

vec3 spotLight(vec3 position, vec3 materialColor, vec3 normal, float specularity, vec3 viewDirection, SpotLight light)
{
	vec3 toLight = normalize(light.position - position);

	float angle = dot(toLight, normalize(-light.direction));

	if(angle > light.openingAngle)
	{
		float specular = blinnSpecular(toLight, normal, viewDirection, 64);

		return diffuse(normal, toLight, light.color, materialColor) + specular * light.color * specularity;
	}
	else
	{
		return vec3(0);
	}
}

vec3 spotLightWithAttenuation(vec3 position, vec3 materialColor, vec3 normal, float specularity, vec3 viewDirection, SpotLight light)
{
	vec3 toLight = normalize(light.position - position);

	float angle = dot(toLight, normalize(-light.direction));

	if(angle > light.openingAngle)
	{
		float dist = length(light.position - position);
		float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

		float specular = blinnSpecular(toLight, normal, viewDirection, 64);
		specular *= attenuation;

		return diffuse(normal, toLight, light.color, materialColor) * attenuation + specular * light.color * specularity;
	}
	else
	{
		return vec3(0);
	}
}

void main() 
{
	vec3 fragPos = texture(gPosition, uv).rgb;
    vec3 normal = texture(gNormal, uv).rgb;
    vec3 albedo = texture(gAlbedoSpec, uv).rgb;
	float specular = texture(gAlbedoSpec, uv).a;
	vec3 viewDirection = normalize(cameraPosition - fragPos);
	
	vec3 finalColor;
	
	finalColor = ambient(ambientLightColor, albedo);

	//Sun
	if (sun.color != vec3(0))
	{
		//finalColor += directionalLight(albedo, normal, vec3(1.0), vec3(1.0), specular * 1.0);
	}

	finalColor += directionalLight(albedo, normal, normalize(vec3(1.0, -1.0, 1.0)), vec3(1.0), specular * 0.0);
	//finalColor += directionalLight(albedo, normal, normalize(vec3(1.0, 0.0, 0.0)), vec3(1.0), specular * 0.0);
	//finalColor += directionalLight(albedo, normal, normalize(vec3(-1.0, 0.0, 0.0)), vec3(1.0), specular * 0.0);
	//
	//finalColor += directionalLight(albedo, normal, normalize(vec3(0.0, 1.0, 0.0)), vec3(1.0), specular * 0.0);
	//finalColor += directionalLight(albedo, normal, normalize(vec3(0.0, -1.0, 0.0)), vec3(0.8, 0.8, 1.0), specular * 0.0);
	//
	//finalColor += directionalLight(albedo, normal, normalize(vec3(0.0, 0.0, 1.0)), vec3(1.0), specular * 0.0);
	//finalColor += directionalLight(albedo, normal, normalize(vec3(0.0, 0.0, -1.0)), vec3(1.0), specular * 0.0);

	//Pointlights
	for(int i = 0; i < NR_LIGHTS; i++)
	{
		if (pointLights[i].color != vec3(0))
		{
			finalColor += pointLightWithAttenuation(fragPos, albedo, normal, specular * pointLights[i].specularStrength, viewDirection, pointLights[i]);
		}
	}

	//Spotlights
	for(int i = 0; i < NR_LIGHTS; i++)
	{
		if (spotLights[i].color != vec3(0))
		{
			finalColor += spotLightWithAttenuation(fragPos, albedo, normal, specular * spotLights[i].specularStrength, viewDirection, spotLights[i]);
		}
	}

	color = vec4(finalColor, 1.0);
}