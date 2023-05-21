#include "LightComponent.h"

#include "Light.h"

#include "Renderer.h"

#include "LoggingSystem.h"
#include <format>

#include "Entity.h"
#include "TransformComponent.h"

namespace MCK::EntitySystem {
LightComponent::LightComponent(LightComponent::LightType a_LightType,
	Eigen::Vector4f a_DiffuseColour, Eigen::Vector4f a_SpecularColour, Eigen::Vector4f a_AmbientColour,
	float a_BeamAngle) :
	m_EntityTransformComponent(nullptr),
	m_PointLight(nullptr), m_DirectionLight(nullptr), m_SpotLight(nullptr),
	m_LightType(LightComponent::LightType::PointLight),
	m_DiffuseColour(a_DiffuseColour), m_SpecularColour(a_SpecularColour), m_AmbientColour(a_AmbientColour), m_BeamAngle(a_BeamAngle) {}
LightComponent::LightComponent() { m_BeamAngle = 50; }
LightComponent::~LightComponent() {}

void LightComponent::OnCreate()
{
	uint32_t tempID = 42;
	std::string tempName = "Answer to the Universe";

	// Load Transform Component from Entity
	m_EntityTransformComponent = entity->GetComponent<TransformComponent>();

	// Check if Entity Transform Component is Valid
	if (!m_EntityTransformComponent)
	{
		Logger::log(std::format("Entity #{} ({}) Light Component Requires an Assigned Transform Component.", tempID, tempName),
			Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return;
	}

	Eigen::AngleAxisf angleAxis(m_EntityTransformComponent->Rotation());
	Eigen::Vector3f direction = angleAxis.axis();

	m_PointLight = new Rendering::PointLight(m_EntityTransformComponent->Position(), m_DiffuseColour, m_SpecularColour, m_AmbientColour);
	m_DirectionLight = new Rendering::DirectionLight(direction, m_DiffuseColour, m_SpecularColour, m_AmbientColour);
	m_SpotLight = new Rendering::SpotLight(m_EntityTransformComponent->Position(), direction, m_BeamAngle, m_DiffuseColour, m_SpecularColour, m_AmbientColour);
}
void LightComponent::OnDestroy()
{

}
void LightComponent::OnUpdate()
{

	m_PointLight->Position() = m_EntityTransformComponent->Position();
	m_SpotLight->Position() = m_EntityTransformComponent->Position();

	Eigen::AngleAxisf angleAxis(m_EntityTransformComponent->Rotation());
	Eigen::Vector3f direction = angleAxis.axis();

	m_DirectionLight->Direction() = direction;
	m_SpotLight->Direction() = direction;

	m_SpotLight->BeamAngle() = m_BeamAngle;

	m_PointLight->DiffuseColour() = m_DiffuseColour;
	m_DirectionLight->DiffuseColour() = m_DiffuseColour;
	m_SpotLight->DiffuseColour() = m_DiffuseColour;

	m_PointLight->SpecularColour() = m_SpecularColour;
	m_DirectionLight->SpecularColour() = m_SpecularColour;
	m_SpotLight->SpecularColour() = m_SpecularColour;

	m_PointLight->AmbientColour() = m_AmbientColour;
	m_DirectionLight->AmbientColour() = m_AmbientColour;
	m_SpotLight->AmbientColour() = m_AmbientColour;

	switch (m_LightType)
	{
	case PointLight:
		Rendering::Renderer::QueuePointLight(m_PointLight);
		break;
	case DirectionLight:
		Rendering::Renderer::QueueDirectionLight(m_DirectionLight);
		break;
	case SpotLight:
		Rendering::Renderer::QueueSpotLight(m_SpotLight);
		break;
	}
}
bool LightComponent::Deserialise(json a_Data)
{
	a_Data = a_Data["data"];

	m_DiffuseColour = Eigen::Vector4f::Ones();
	m_SpecularColour = Eigen::Vector4f::Ones();
	m_AmbientColour = Eigen::Vector4f::Zero();

	for (auto itt = a_Data.begin(); itt != a_Data.end(); ++itt)
	{
		if (itt.key() == "type")
		{
			int lt = a_Data["type"];
			m_LightType = (LightType)lt;
		}
		else if (itt.key() == "diffuse")
		{
			m_DiffuseColour.x() = a_Data["diffuse"][0];
			m_DiffuseColour.y() = a_Data["diffuse"][1];
			m_DiffuseColour.z() = a_Data["diffuse"][2];
			m_DiffuseColour.w() = a_Data["diffuse"][3];
		}
	}
	return true;
}
}
