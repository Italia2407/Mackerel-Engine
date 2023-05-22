#version 450 core

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;
layout(location = 4) in vec3 v2fCameraViewDirection;

// Required Geometry Buffer Parameters
layout(location = 0) out uint gLightShaderID;

// Define Here your Geometry Buffer Output Parameters
layout(location = 1) out vec4 gAlbedoColour;

layout(location = 2) out vec4 gPosition;
layout(location = 3) out vec4 gNormal;

layout( binding = 0) uniform sampler2D textureSampler;

layout(std140, binding = 2) uniform MaterialParameters
{
	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
};

void main()
{
	gLightShaderID = 1; //lightShaderID;

	gAlbedoColour = texture(textureSampler, v2fUV);
	//gAlbedoColour = vec4(ambientColour, 1.0f);

	gPosition = vec4(v2fPosition, 1.0f);
	gNormal = vec4(v2fNormal, 1.0f);
}
