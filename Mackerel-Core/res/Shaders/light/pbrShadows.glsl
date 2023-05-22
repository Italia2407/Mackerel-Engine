#version 450

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;
layout(location = 4) in vec3 v2fCameraViewDirection;

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
	vec4 attenuationParams;

	vec4 diffuseColour;
	vec4 specularColour;
	vec4 ambientColour;
} light;

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

layout(binding = 4) uniform sampler2D gMAR;
layout(binding = 5) uniform sampler2D gCameraViewDirection;

#define PI (3.1415926538f)
#define EPS (0.0000001f)

vec3 xyz_to_rgb(vec3 aXYZ)
{
	return (aXYZ + vec3(1.0f, 1.0f, 1.0f)) / 2.0f;
}
vec3 rgb_to_xyz(vec3 aRGB)
{
	return (aRGB * 2.0f) - vec3(1.0f, 1.0f, 1.0f);
}
float positive_dot(vec3 aVecA, vec3 aVecB)
{
	return max(0.0f, dot(aVecA, aVecB));
}

void main()
{
	oColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	AssertLightShader(2);

	// Extract Fragment Parameters
	vec4 colour = texture(gAlbedoColour, v2fUV);

	vec3 position = texture(gPosition, v2fUV).xyz;
	vec3 normal = texture(gNormal, v2fUV).xyz;

	vec3 mar = texture(gMAR, v2fUV).rgb;

	oColour += colour * vec4(light.ambientColour.rgb, 0.0f);

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

	vec3 cameraViewDirection = texture(gCameraViewDirection, v2fUV).xyz;
	vec3 halfwayDirection = normalize(-light.direction.xyz - cameraViewDirection);

	// Lighting Computations
	vec3 baseReflectivity = ((1.0f - mar.r) * vec3(0.004f, 0.004f, 0.004f)) + (mar.r * colour);
	vec3 fresnelValue = baseReflectivity + (vec3(1.0f, 1.0f, 1.0f) - baseReflectivity) * pow(1.0f - dot(halfwayDirection, -cameraViewDirection), 5.0f);

	vec3 diffuseColour = (colour / PI) * (vec3(1.0f, 1.0f, 1.0f) - fresnelValue) * (1.0f - mar.r);

	float shininess = (2.0f / (pow(mar.b, 4.0f) + EPS)) - 2.0f;
	float normalDistribution = ((shininess + 2) / (2 * PI)) * pow(positive_dot(normal, halfwayDirection), shininess);

	float viewMask = (positive_dot(normal, halfwayDirection) * positive_dot(normal, -cameraViewDirection)) / (dot(-cameraViewDirection, halfwayDirection) + EPS);
	float lightMask = (positive_dot(normal, halfwayDirection) * positive_dot(normal, -light.direction.xyz)) / (dot(-cameraViewDirection, halfwayDirection) + EPS);
	float maskingValue = min(1.0f, min(2.0f * viewMask, 2.0f * lightMask));

	vec3 brdfValue = diffuseColour + ((normalDistribution * fresnelValue * maskingValue) / ((4 * positive_dot(normal, -cameraViewDirection) * positive_dot(normal, -light.direction.xyz)) + EPS));

	vec3 diffuse = brdfValue * light.diffuseColour.rgb * positive_dot(normal, -light.direction.xyz);

	oColour += diffuse;
}