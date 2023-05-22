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

layout(location = 4) out vec4 gMAR;
layout(location = 5) out vec4 gCameraViewDirection;

layout(binding = 0) uniform sampler2D textureSampler;
layout(binding = 1) uniform sampler2d marTextureSampler;

void main()
{
	gLightShaderID = 2; //lightShaderID;

	gAlbedoColour = texture(textureSampler, v2fUV);

	gPosition = vec4(v2fPosition, 1.0f);
	gNormal = vec4(v2fNormal, 1.0f);

	gMAR = texture(marTextureSampler, v2fUV);
	gCameraViewDirection = vec4(v2fCameraViewDirection, 1.0f);
}
