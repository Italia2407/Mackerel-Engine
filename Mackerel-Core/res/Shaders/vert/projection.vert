#version 450 core

// Input Vertex Data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTint;

// Output Fragment Data
layout(location = 0) out vec3 v2fPosition;
layout(location = 1) out vec3 v2fNormal;
layout(location = 2) out vec2 v2fUV;
layout(location = 3) out vec3 v2fTint;

// Camera Uniform Buffer Object
layout(std140, binding = 0) uniform Camera
{
	vec3 position;

	vec3 front;
	vec3 up;

	mat4 cameraProjectionMatrix;
} camera;

// Vertex Shader Uniform Buffer Object
layout(std140, binding = 1) uniform MeshTransform
{
	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 transformMatrix;
} mesh;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0f);

	v2fPosition = vertexPosition;
	v2fNormal = vertexNormal;
	v2fUV = vertexUV;
	v2fTint = (camera.up + mesh.scale) * 0.5f;
	//v2fTint = vertexTint;
}