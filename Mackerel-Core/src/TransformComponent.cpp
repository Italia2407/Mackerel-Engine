#include "TransformComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>

namespace MCK::EntitySystem
{
/**
* Returns Transformation Matrix of Entity
* Takes Parent Entity Transformations into Account
*/
Eigen::Matrix4f TransformComponent::GetTransformationMatrix() const
{
	// Get Parent Entity's Transformation Matrix, if Existstant
	Eigen::Matrix4f parentMatrix = Eigen::Matrix4f::Identity();

	if (entity->parent != nullptr) {
		TransformComponent* parentTransform = entity->parent->GetComponent<TransformComponent>();
		if (parentTransform != nullptr)
			parentMatrix = parentTransform->GetTransformationMatrix();
	}

	return parentMatrix * m_Transform.Matrix();
}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TransformComponent::OnCreate()
	{
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TransformComponent::OnUpdate()
	{

	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void TransformComponent::OnDestroy()
	{
	}

bool TransformComponent::Deserialise(json data)
{
	// Get Entity's Transform Component Data.
	std::string j = data.dump();
	data = data["data"];

	// Get Transform Component's Position
	Position().x() = data["positionX"];
	Position().y() = data["positionY"];
	Position().z() = data["positionZ"];

	// Get Transform Component's Rotation
	auto it = data.find("rotationX");
	if (it != data.end())
	{
		Rotation().x() = data["rotationX"];
		Rotation().y() = data["rotationY"];
		Rotation().z() = data["rotationZ"];
		Rotation().w() = data["rotationW"];
	}


	// Get Transform Component's Scale
	it = data.find("scaleX");
	if (it != data.end())
	{
		Scale().x() = data["scaleX"];
		Scale().y() = data["scaleY"];
		Scale().z() = data["scaleZ"];
	}
	else
	{
		Scale() = Eigen::Vector3f(1, 1, 1);
	}


	// Get Transform Component's Shear
	it = data.find("YZPlaneShearY");
	if (it != data.end())
	{
		YZPlaneShear().x() = data["YZPlaneShearY"];
		YZPlaneShear().y() = data["YZPlaneShearZ"];
	}

	it = data.find("XZPlaneShearX");
	if (it != data.end())
	{
		XZPlaneShear().x() = data["XZPlaneShearX"];
		XZPlaneShear().y() = data["XZPlaneShearZ"];
	}

	it = data.find("XYPlaneShearX");
	if (it != data.end())
	{
		XYPlaneShear().x() = data["XYPlaneShearX"];
		XYPlaneShear().y() = data["XYPlaneShearY"];
	}


	return true;
}
}