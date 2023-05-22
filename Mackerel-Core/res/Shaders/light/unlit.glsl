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
	AssertLightShader(0);
	oColour = vec4(texture(gAlbedoColour, v2fUV).rgb, 1.0f);
}