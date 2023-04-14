#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Forward Declarations
namespace MCK {
class UniformBuffer;
}

namespace MCK::AssetType {
class Texture;
}

namespace MCK::Rendering {
class Light
{
public:
	Light(glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour);

	AssetType::Texture* ShadowMap() const { return _shadowMap; }

protected:
	glm::vec4 _diffuseColour;
	glm::vec4 _specularColour;
	glm::vec4 _ambientColour;

	AssetType::Texture* _shadowMap;
	UniformBuffer* _parametersBuffer;

	virtual glm::mat4 getMVPMatrix();

	virtual bool updateLightingParameters() = 0;

public:
	bool UseLight();
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour);

private:
	glm::vec3 _position;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
class DirectionLight : public Light
{
public:
	DirectionLight(glm::vec3 direction, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour);

private:
	glm::vec3 _direction;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
class SpotLight : public Light
{
public:
	SpotLight(glm::vec3 position, glm::vec3 direction, float beamAngle, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour);

private:
	glm::vec3 _position;
	glm::vec3 _direction;

	float _beamAngle;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
}
