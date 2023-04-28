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
	mat4 transformMatrix;
} mesh;

void main()
{
	// Compute Transformed Position
	vec4 transformedPosition = mesh.transformMatrix * vec4(vertexPosition, 1.0f);
	v2fPosition = transformedPosition.xyz / transformedPosition.w;

	// Compute Transformed Normal
	vec4 transformedNormal = transpose(inverse(mesh.transformMatrix)) * vec4(vertexNormal, 1.0f);
	v2fNormal = normalize(transformedNormal.xyz);

	gl_Position = camera.cameraProjectionMatrix * transformedPosition;

	v2fUV = vertexUV;
	v2fTint = vertexTint;
}