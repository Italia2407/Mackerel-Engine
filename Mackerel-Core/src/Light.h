#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

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
	Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

	AssetType::Texture* ShadowMap() const { return _shadowMap; }

protected:
	Eigen::Vector4f _diffuseColour;
	Eigen::Vector4f _specularColour;
	Eigen::Vector4f _ambientColour;

	AssetType::Texture* _shadowMap;
	UniformBuffer* _parametersBuffer;

	virtual Eigen::Matrix4f getMVPMatrix();

	virtual bool updateLightingParameters() = 0;

public:
	bool UseLight();
};

class PointLight : public Light
{
public:
	PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

private:
	Eigen::Vector3f _position;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
class DirectionLight : public Light
{
public:
	DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

private:
	Eigen::Vector3f _direction;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
class SpotLight : public Light
{
public:
	SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

private:
	Eigen::Vector3f _position;
	Eigen::Vector3f _direction;

	float _beamAngle;

	//Eigen::Matrix4f getMVPMatrix() override;

	bool updateLightingParameters() override;
};
}
