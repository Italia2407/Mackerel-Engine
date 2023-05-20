#version 450

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;

// Output Fragment Data
layout(location = 0) out vec4 oColour;

// Required Geometry Buffer Parameters
layout(binding = 0) uniform usampler2D gLightShaderID;

layout(binding = 31) uniform sampler2D shadowMap;

// Light Parameters
layout(std140, binding = 3) uniform LightParameters
{
	mat4 modelViewProjection;

	vec4 position;
	vec4 direction;

	float beamAngle;

	vec4 diffuseColour;
	vec4 specularColour;
	vec4 ambientColour;
} light;

layout(std140, binding = 4) uniform LightParams
{
	float beamAngle;

	vec3 diffuseColour;
	vec3 specularColour;
	vec3 ambientColour;
} lightParams;
// Function to Assert Correct Light Shader Used
void AssertLightShader(uint a_LightShaderID)
{
	if (texture(gLightShaderID, v2fUV).r != a_LightShaderID)
	{
		discard;
	}
}

// Define Here your Geometry Buffer Output Parameters
layout(binding = 1) uniform sampler2D gAlbedoColour;

layout(binding = 2) uniform sampler2D gPosition;
layout(binding = 3) uniform sampler2D gNormal;

void main()
{
	AssertLightShader(1);

	// Extract Fragment Parameters
	vec4 colour = texture(gAlbedoColour, v2fUV);

	vec3 position = texture(gPosition, v2fUV).xyz;
	vec3 normal = texture(gNormal, v2fUV).xyz;

	oColour = colour * vec4(light.ambientColour.rgb, 1.0f);

	// Compare Distance with Shadow-Map
	vec3 shadowCoord = (light.modelViewProjection * vec4(position, 1.0f)).xyz;
	vec3 normShadowCoord = (shadowCoord + vec3(1.0f, 1.0f, 1.0f)) / 2.0f;

	float occluderDistance = texture(shadowMap, normShadowCoord.xy).z;
	if (normShadowCoord.z > occluderDistance)
		return;

	// Compare Light Direction with normal
	float incidenceAmount = dot(normal, -light.direction.xyz);
	if (incidenceAmount <= 0.0f)
		return;

	oColour = colour * ((incidenceAmount * light.diffuseColour) + vec4(light.ambientColour.rgb, 1.0f));
}