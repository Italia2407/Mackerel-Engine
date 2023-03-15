////#extension GL_EXT_gpu_shader4 : require
//// Light Shader Output
//out vec4 FragmentColour;
//// Light Shader Inputs
//in vec2 TextureCoords;
//
//layout(binding = 0) uniform usampler2D gLightShaderID;
//layout(binding = 31) uniform sampler2D shadowMap;

// Light Parameters
//layout(binding = 0) uniform LightParameters
//{
// mat4 modelViewProjection;
//
//	vec3 position;
//	vec3 direction;
//
//	float beamAngle;
//
//	vec4 diffuseColour;
//	vec4 specularColour;
//	vec4 ambientColour;
//} light;

//void AssertUsage(uint shaderID)
//{
//	uint lightShaderID = texture(gLightShaderID, TextureCoords).a;
//	if (shaderID != lightShaderID)
//		discard;
//}

//float getShadowMapDepth(vec3 position)
//{
//	vec4 shadowCoord = light.MVP * vec4(position, 1.0f);
//	float shadowMapDepth = texture(shadowMap, shadowCoord.xy).r;
//
//	return shadowMapDepth;
//}