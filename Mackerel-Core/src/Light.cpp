#include "Light.h"

#include "FrameBuffer.h"
#include "UniformBuffer.h"

#include "Texture.h"

#include <iostream>

namespace MCK::Rendering {
Light::Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	_diffuseColour(diffuseColour), _specularColour(specularColour), _ambientColour(ambientColour),
	m_ShadowRenderer(nullptr), m_LightParameters(nullptr), m_ShadowRendererParameters(nullptr)
{
	// Initialise & Create Shadow Map Renderer Framebuffer
	m_ShadowRenderer = new FrameBuffer(8192, 8192);

	m_ShadowRenderer->AddDepthBufferTexture();
	m_ShadowRenderer->CreateFrameBuffer();

	// Initialse & Create Light Parameters Buffer
	m_LightParameters = new UniformBuffer(); {
		// Add Light ModelViewProjection
		m_LightParameters->AddMat4BufferUniform("modelViewProjection", Eigen::Matrix4f::Identity());

		// Add Position & Direction
		m_LightParameters->AddVec3BufferUniform("position", Eigen::Vector3f::Zero());
		m_LightParameters->AddVec3BufferUniform("direction", Eigen::Vector3f::Zero());

		// Add Beam Angle
		m_LightParameters->AddFloatBufferUniform("beamAngle", 0.0f);

		// Add Light Colours
		m_LightParameters->AddVec4BufferUniform("diffuseColour", Eigen::Vector4f::Zero());
		m_LightParameters->AddVec4BufferUniform("specularColour", Eigen::Vector4f::Zero());
		m_LightParameters->AddVec4BufferUniform("ambientColour", Eigen::Vector4f::Zero());
	}
	m_LightParameters->CreateUniformBufferObject();

	// Initialise & Create Shadow Renderer Parameter Object
	m_ShadowRendererParameters = new UniformBuffer(); {
		m_ShadowRendererParameters->AddVec3BufferUniform("position", Eigen::Vector3f::Zero());

		m_ShadowRendererParameters->AddVec3BufferUniform("front", Eigen::Vector3f::Zero());
		m_ShadowRendererParameters->AddVec3BufferUniform("up", Eigen::Vector3f(1.0f, 0.0f, 0.0f));

		m_ShadowRendererParameters->AddMat4BufferUniform("cameraProjectionMatrix", Eigen::Matrix4f::Identity());
	}
	m_ShadowRendererParameters->CreateUniformBufferObject();
}

bool Light::UseLight(Eigen::Vector3f a_CentrePosition)
{
	updateLightingParameters(a_CentrePosition);

	// Bind Parameters Buffer
	m_LightParameters->BindUniformBufferObject(3);

	// Bind Shadow Map
	m_ShadowRenderer->GetInternalDepthBufferTexture()->BindTexture(31);

	return true;
}

PointLight::PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _position(position) {}
DirectionLight::DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _direction(direction) {}
SpotLight::SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _position(position), _direction(direction), _beamAngle(beamAngle) {}

Eigen::Matrix4f PointLight::getMVPMatrix(Eigen::Vector3f a_CentrePosition)
{
	return Eigen::Matrix4f::Identity();
}
Eigen::Matrix4f DirectionLight::getMVPMatrix(Eigen::Vector3f a_CentrePosition)
{
	Eigen::Vector3f position = (-50.0f * _direction) + a_CentrePosition;

	Eigen::Vector3f front = _direction;
	Eigen::Vector3f up = Eigen::Vector3f(1.0f, 0.0f, 0.0f);

	// Calculate the Camera Space Axes Directions
	Eigen::Vector3f cameraZAxis = Eigen::Vector3f(front.x(), front.y(), front.z()).normalized();
	Eigen::Vector3f cameraXAxis = cameraZAxis.cross(Eigen::Vector3f(up.x(), up.y(), up.z())).normalized();
	Eigen::Vector3f cameraYAxis = cameraXAxis.cross(cameraZAxis).normalized();

	// Create the Camera View Matrix
	Eigen::Matrix4f cameraViewMatrix = Eigen::Matrix4f::Identity(); {
		cameraViewMatrix.coeffRef(0, 0) = cameraXAxis.x();
		cameraViewMatrix.coeffRef(0, 1) = cameraXAxis.y();
		cameraViewMatrix.coeffRef(0, 2) = cameraXAxis.z();
		cameraViewMatrix.coeffRef(0, 3) = cameraXAxis.dot(-position);

		cameraViewMatrix.coeffRef(1, 0) = cameraYAxis.x();
		cameraViewMatrix.coeffRef(1, 1) = cameraYAxis.y();
		cameraViewMatrix.coeffRef(1, 2) = cameraYAxis.z();
		cameraViewMatrix.coeffRef(1, 3) = cameraYAxis.dot(-position);

		cameraViewMatrix.coeffRef(2, 0) = cameraZAxis.x();
		cameraViewMatrix.coeffRef(2, 1) = cameraZAxis.y();
		cameraViewMatrix.coeffRef(2, 2) = cameraZAxis.z();
		cameraViewMatrix.coeffRef(2, 3) = cameraZAxis.dot(-position);
	}

	Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Identity(); {
		projectionMatrix.coeffRef(0, 0) = 2.0f / (20.0f);

		projectionMatrix.coeffRef(1, 1) = 2.0f / (20.0f);

		projectionMatrix.coeffRef(2, 2) = 1.0f / (100.0f - 0.0001f);
		projectionMatrix.coeffRef(2, 3) = -(0.0001f) / (100.0f - 0.0001f);
	}

	return projectionMatrix * cameraViewMatrix;
}
Eigen::Matrix4f SpotLight::getMVPMatrix(Eigen::Vector3f a_CentrePosition)
{
	return Eigen::Matrix4f::Identity();
}

bool PointLight::updateLightingParameters(Eigen::Vector3f a_CentrePosition)
{
	// Set ModelViewProjection
	m_LightParameters->SetMat4BufferUniform("modelViewProjection", getMVPMatrix(a_CentrePosition));

	// Set Position
	m_LightParameters->SetVec3BufferUniform("position", _position);

	// Set Colours
	m_LightParameters->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	m_LightParameters->SetVec4BufferUniform("specularColour", _specularColour);
	m_LightParameters->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}
bool DirectionLight::updateLightingParameters(Eigen::Vector3f a_CentrePosition)
{
	// Set ModelViewProjection
	m_LightParameters->SetMat4BufferUniform("modelViewProjection", getMVPMatrix(a_CentrePosition));

	// Set Direction
	m_LightParameters->SetVec3BufferUniform("direction", _direction);

	// Set Colours
	m_LightParameters->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	m_LightParameters->SetVec4BufferUniform("specularColour", _specularColour);
	m_LightParameters->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}
bool SpotLight::updateLightingParameters(Eigen::Vector3f a_CentrePosition)
{
	// Set ModelViewProjection
	m_LightParameters->SetMat4BufferUniform("modelViewProjection", getMVPMatrix(a_CentrePosition));

	// Set Position & Direction
	m_LightParameters->SetVec3BufferUniform("position", _position);
	m_LightParameters->SetVec3BufferUniform("direction", _direction);

	// Set Beam Angle
	m_LightParameters->SetFloatBufferUniform("beamAngle", _beamAngle);

	// Set Colours
	m_LightParameters->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	m_LightParameters->SetVec4BufferUniform("specularColour", _specularColour);
	m_LightParameters->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}

bool DirectionLight::BindShadowRendererCamera(Eigen::Vector3f a_CentrePosition)
{
	m_ShadowRendererParameters->BindUniformBufferObject(0);

	// Set Camera Position
	m_ShadowRendererParameters->SetVec3BufferUniform("position", (-1000.0f * _direction) + a_CentrePosition);
	// Set Camera Front Direction
	m_ShadowRendererParameters->SetVec3BufferUniform("front", _direction);
	// Set Camera MVP Matrix
	m_ShadowRendererParameters->SetMat4BufferUniform("cameraProjectionMatrix", getMVPMatrix(a_CentrePosition));

	return true;
}
}
