#version 450 core

// Input Vertex Data
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec4 vertexColour;

// Mesh Instance Parameters
struct Transform
{
	vec3 meshPosition;
	vec3 meshScale;
	vec4 meshRotation;
};
uniform Transform meshTransform;

void main()
{

}