#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

// Forward Declarations
namespace MCK {
class UniformBuffer;
class FrameBuffer;
}

namespace MCK::AssetType {
class Texture;
}

namespace MCK::Rendering {
class Light
{
public:
	Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

protected:
	Eigen::Vector4f _diffuseColour;
	Eigen::Vector4f _specularColour;
	Eigen::Vector4f _ambientColour;

	FrameBuffer* m_ShadowRenderer;

	UniformBuffer* m_LightParameters;
	UniformBuffer* m_LightParams;
	UniformBuffer* m_ShadowRendererParameters;

	virtual Eigen::Matrix4f getMVPMatrix(Eigen::Vector3f a_CentrePosition) = 0;
	virtual bool updateLightingParameters(Eigen::Vector3f a_CentrePosition) = 0;

public:
	FrameBuffer* ShadowRenderer() { return m_ShadowRenderer; }

	bool UseLight(Eigen::Vector3f a_CentrePosition);
	virtual bool BindShadowRendererCamera(Eigen::Vector3f a_CentrePosition) = 0;
};

class PointLight : public Light
{
public:
	PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

protected:
	Eigen::Vector3f _position;

	Eigen::Matrix4f getMVPMatrix(Eigen::Vector3f a_CentrePosition) override;
	bool updateLightingParameters(Eigen::Vector3f a_CentrePosition) override;
};
class DirectionLight : public Light
{
public:
	DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

protected:
	Eigen::Vector3f _direction;

	Eigen::Matrix4f getMVPMatrix(Eigen::Vector3f a_CentrePosition) override;
	bool updateLightingParameters(Eigen::Vector3f a_CentrePosition) override;

public:
	bool BindShadowRendererCamera(Eigen::Vector3f a_CentrePosition) override;
};
class SpotLight : public Light
{
public:
	SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

protected:
	Eigen::Vector3f _position;
	Eigen::Vector3f _direction;

	float _beamAngle;

	Eigen::Matrix4f getMVPMatrix(Eigen::Vector3f a_CentrePosition) override;
	bool updateLightingParameters(Eigen::Vector3f a_CentrePosition) override;
};
}
