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
Eigen::Matrix4f TransformComponent::GetTransformationMatrix()
{
	// Get Parent Entity's Transformation Matrix, if Existstant
	Eigen::Matrix4f parentMatrix;
	parentMatrix.Identity();

	if (entity->parent != nullptr) {
		TransformComponent* parentTransform = entity->parent->GetComponent<TransformComponent>();
		if (parentTransform != nullptr)
			parentMatrix = parentTransform->GetTransformationMatrix();
	}

	// Apply Affine Transformations
	Eigen::Affine3f affineTransformations;
	affineTransformations.translate(m_Position);
	affineTransformations.rotate(m_Rotation);
	affineTransformations.scale(m_Scale);

	// Create Shear Matrix
	Eigen::Matrix4f shearMatrix;
	shearMatrix.Identity();

	// Set X-Axis Shear
	shearMatrix(0, 1) = m_YZPlaneShear.x();
	shearMatrix(0, 2) = m_YZPlaneShear.y();
	// Set Y-Axis Shear
	shearMatrix(1, 0) = m_XZPlaneShear.x();
	shearMatrix(1, 2) = m_XZPlaneShear.y();
	// Set Z-Axis Shear
	shearMatrix(2, 0) = m_XYPlaneShear.x();
	shearMatrix(2, 1) = m_XYPlaneShear.y();

	Eigen::Matrix4f transformationMatrix;
	// Order matters, so this multiplication not be correct.
	transformationMatrix = parentMatrix * affineTransformations.matrix() * shearMatrix;

	return transformationMatrix;
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
	m_Position.x() = data["positionX"];
	m_Position.y() = data["positionY"];
	m_Position.z() = data["positionZ"];

	// Get Transform Component's Rotation
	m_Rotation.x() = data["rotationAxisX"];
	m_Rotation.y() = data["rotationAxisY"];
	m_Rotation.z() = data["rotationAxisZ"];
	m_Rotation.w() = data["rotationAmount"];

	// Get Transform Component's Scale
	m_Scale.x() = data["scaleX"];
	m_Scale.y() = data["scaleY"];
	m_Scale.z() = data["scaleZ"];

	// Get Transform Component's Shear
	m_YZPlaneShear.x() = data["YZPlaneShearY"];
	m_YZPlaneShear.y() = data["YZPlaneShearZ"];

	m_XZPlaneShear.x() = data["XZPlaneShearX"];
	m_XZPlaneShear.y() = data["XZPlaneShearZ"];

	m_XYPlaneShear.x() = data["XYPlaneShearX"];
	m_XYPlaneShear.y() = data["XYPlaneShearY"];

	return true;
}
}