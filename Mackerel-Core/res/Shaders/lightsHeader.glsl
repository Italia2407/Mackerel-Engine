////#extension GL_EXT_gpu_shader4 : require
//#define MAX_LIGHTS (32)
//
//struct PointLight
//{							// Base Allignment		// Alligned Offset
//	vec4 diffuseColour;		// 16					// 0
//	vec4 specularColour;	// 16					// 16
//	vec4 ambientColour;		// 16					// 32
//
//	vec3 position;			// 16					// 48
//};							// 64
//struct DirectionLight
//{							// Base Allignment		// Alligned Offset
//	vec4 diffuseColour;		// 16					// 0
//	vec4 specularColour;	// 16					// 16
//	vec4 ambientColour;		// 16					// 32
//
//	vec3 direction;			// 16					// 84
//};							// 64
//struct SpotLight
//{							// Base Allignment		// Alligned Offset
//	vec4 diffuseColour;		// 16					// 0
//	vec4 specularColour;	// 16					// 16
//	vec4 ambientColour;		// 16					// 32
//
//	vec3 position;			// 16					// 48
//	vec3 direction;			// 16					// 64
//
//	float beamAngle;		// 4					// 68
//};							// 84
//
//// Light Shader Output
//out vec4 FragmentColour;
//// Light Shader Inputs
//in vec2 TextureCoords;
//
//layout(binding = 0) uniform usampler2D gLightShaderID;
//
//// Light Parameters Uniform Buffer Object
//layout(std140, binding = 0) uniform LightParameters
//{												// Base Allignment				// Alligned Offset
//	uint numPointLights;						// 4							// 0
//	uint numDirectionLights;					// 4							// 4
//	uint numSpotLights;							// 4							// 8
//
//	PointLight pointLights[MAX_LIGHTS];			// 2048							// 12
//	DirectionLight directionLights[MAX_LIGHTS]; // 2048							// 2060
//	SpotLight spotLights[MAX_LIGHTS];			// 2560							// 4108
//};												// 6,668
//
//void AssertUsage(uint shaderID)
//{
//	uint lightShaderID = texture(gLightShaderID, TextureCoords).a;
//	if (shaderID != lightShaderID)
//		discard;
//}