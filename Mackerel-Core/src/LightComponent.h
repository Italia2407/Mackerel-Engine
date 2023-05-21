#pragma once
#include "Component.h"

#include <Eigen/Eigen.h>

namespace MCK::Rendering {
class Light;

class PointLight;
class DirectionLight;
class SpotLight;
}

namespace MCK::EntitySystem {
class TransformComponent;
}

namespace MCK::EntitySystem {
class LightComponent : public Component<LightComponent>
{
public:
	enum LightType
	{
		PointLight,
		DirectionLight,
		SpotLight
	};

	LightComponent();
	LightComponent(LightType a_LightType,
		Eigen::Vector4f a_DiffuseColour = Eigen::Vector4f::Zero(), Eigen::Vector4f a_SpecularColour = Eigen::Vector4f::Zero(), Eigen::Vector4f a_AmbientColour = Eigen::Vector4f::Zero(),
		float a_BeamAngle = 0.0f);
	~LightComponent();

private:
	// Reference to Entity Transform Component
	TransformComponent* m_EntityTransformComponent;

	// Reference to Light Pointers
	Rendering::PointLight* m_PointLight;
	Rendering::DirectionLight* m_DirectionLight;
	Rendering::SpotLight* m_SpotLight;

	LightType m_LightType;

	Eigen::Vector4f m_DiffuseColour;
	Eigen::Vector4f m_SpecularColour;
	Eigen::Vector4f m_AmbientColour;

	float m_BeamAngle;

public:
	void SetDiffuseColour(Eigen::Vector4f a_DiffuseColour);
	void SetSpecularColour(Eigen::Vector4f a_SpecularColour);
	void SetAmbientColour(Eigen::Vector4f a_AmbientColour);

	void SetBeamAngle(float a_BeamAngle);

public:

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate() override;

	bool Deserialise(json a_Data) override;
};
}
