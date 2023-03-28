#version 450 core

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;

// Required Geometry Buffer Parameters
layout(location = 1) out uint gLightShaderID;

// Define Here your Geometry Buffer Output Parameters
layout(location = 0) out vec4 gAlbedoColour;

layout(location = 2) out vec4 gPosition;
layout(location = 3) out vec4 gNormal;

layout(std140, binding = 2) uniform MaterialParameters
{
	uint lightShaderID;
	vec3 albedoColour;
};

void main()
{
	gLightShaderID = 0; //lightShaderID;

	//gAlbedoColour = albedoColour * v2fTint;
	gAlbedoColour = vec4(v2fUV, 0.75f, 1.0f);

	gPosition = vec4(v2fPosition, 1.0f);
	gNormal = vec4(v2fNormal, 1.0f);
}
