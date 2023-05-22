#version 450 core

// Input Fragment Data
layout(location = 0) in vec3 v2fPosition;
layout(location = 1) in vec3 v2fNormal;
layout(location = 2) in vec2 v2fUV;
layout(location = 3) in vec3 v2fTint;
layout(location = 4) in v2fCameraViewDirection;

// Output Fragment Data
layout(location = 0) out vec3 oColour;

layout(binding = 0) uniform sampler2D FBTexture;

void main()
{
	oColour = texture(FBTexture, v2fUV).rgb;
}