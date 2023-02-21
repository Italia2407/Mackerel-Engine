#version 450 core
#define MAX_LIGHTS (32)

uniform	sampler2D gLightShaderID;

uniform	sampler2D gAlbedo;

uniform	sampler2D gWorldPosition;
uniform	sampler2D gNormal;

struct PointLight
{							// Base Allignment		// Alligned Offset
	vec3 position;			// 16					// 0

	vec4 diffuseColour;		// 16					// 16
	vec4 specularColour;	// 16					// 32
	vec4 ambientColour;		// 16					// 48
};							// 64
struct DirectionLight
{							// Base Allignment		// Alligned Offset
	vec3 direction;			// 16					// 0

	vec4 diffuseColour;		// 16					// 16
	vec4 specularColour;	// 16					// 32
	vec4 ambientColour;		// 16					// 48
};							// 64
struct SpotLight
{							// Base Allignment		// Alligned Offset
	vec3 position;			// 16					// 0
	vec3 direction;			// 16					// 16

	vec4 diffuseColour;		// 16					// 32
	vec4 specularColour;	// 16					// 48
	vec4 ambientColour;		// 16					// 64
};							// 80

layout(std140, binding = 0) uniform LightParameters
{												// Base Allignment				// Alligned Offset
	uint numPointLights;						// 4							// 0
	uint numDirectionLights;					// 4							// 4
	uint numSpotLights;							// 4							// 8

	PointLight pointLights[MAX_LIGHTS];			// 2048							// 12
	DirectionLight directionLights[MAX_LIGHTS]; // 2048							// 2060
	SpotLight spotLights[MAX_LIGHTS];			// 2560							// 4108
};												// 6,668

void main()
{

}