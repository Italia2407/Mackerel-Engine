#version 450 core

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;

// Required Geometry Buffer Parameters
layout(binding = 0) out uint gLightShaderID;

// Define Here your Geometry Buffer Output Parameters
layout(binding = 1) out vec3 gAlbedoColour;

layout(binding = 2) out vec3 gPosition;
layout(binding = 3) out vec3 gNormal;

layout(std140, binding = 2) uniform MaterialParameters
{
	uint lightShaderID;
	vec3 albedoColour;
};

void main()
{
	gLightShaderID = lightShaderID;

	gAlbedoColour = albedoColour * v2fTint;

	gPosition = v2fPosition;
	gNormal = v2fNormal;
}
