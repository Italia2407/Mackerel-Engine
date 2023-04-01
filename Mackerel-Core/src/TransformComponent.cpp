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

	std::cout << "Matrix " << m_Transform.Matrix() << std::endl;

	return parentMatrix * m_Transform.Matrix();
}

	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef TransformComponent::GetType()
	{
		return typeid(TransformComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TransformComponent::OnCreate()
	{
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TransformComponent::OnUpdate()
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void TransformComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

bool TransformComponent::Deserialise(json data)
{
	// Get Entity's Transform Component Data.
	data = data["data"];

	// Get Transform Component's Position
	Position().x() = data["positionX"];
	Position().y() = data["positionY"];
	Position().z() = data["positionZ"];

	// Get Transform Component's Rotation
	Rotation().x() = data["rotationAxisX"];
	Rotation().y() = data["rotationAxisY"];
	Rotation().z() = data["rotationAxisZ"];
	Rotation().w() = data["rotationAmount"];

	// Get Transform Component's Scale
	Scale().x() = data["scaleX"];
	Scale().y() = data["scaleY"];
	Scale().z() = data["scaleZ"];

	// Get Transform Component's Shear
	YZPlaneShear().x() = data["YZPlaneShearY"];
	YZPlaneShear().y() = data["YZPlaneShearZ"];

	XZPlaneShear().x() = data["XZPlaneShearX"];
	XZPlaneShear().y() = data["XZPlaneShearZ"];

	XYPlaneShear().x() = data["XYPlaneShearX"];
	XYPlaneShear().y() = data["XYPlaneShearY"];

	return true;
}
}