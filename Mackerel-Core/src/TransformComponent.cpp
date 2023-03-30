#include "TransformComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>

namespace MCK::EntitySystem
{
	/**
	* Returns the transformation matrix of this entity
	* Takes the transformation of parent entities into account
	* 
	* 
	*/
	Eigen::Matrix4f TransformComponent::GetTransformationMatrix()
	{
		Eigen::Matrix4f translateMat;
		translateMat.Identity();
		translateMat(0,3) = position.x();
		translateMat(1,3) = position.y();
		translateMat(2,3) = position.z();

		Eigen::Matrix4f rotateXMat;
		rotateXMat.Identity();
		rotateXMat(1,1) = cos(eulerAngles.x());
		rotateXMat(1,2) = -sin(eulerAngles.x());
		rotateXMat(2,1) = sin(eulerAngles.x());
		rotateXMat(2,2) = cos(eulerAngles.x());
		
		Eigen::Matrix4f rotateYMat;
		rotateYMat.Identity();
		rotateYMat(0,0) = cos(eulerAngles.y());
		rotateYMat(0,2) = sin(eulerAngles.y());
		rotateYMat(2,0) = -sin(eulerAngles.y());
		rotateYMat(2,2) = cos(eulerAngles.y());
		
		Eigen::Matrix4f rotateZMat;
		rotateZMat.Identity();
		rotateZMat(0,0) = cos(eulerAngles.z());
		rotateZMat(0,1) = -sin(eulerAngles.z());
		rotateZMat(1,0) = sin(eulerAngles.z());
		rotateZMat(1,1) = cos(eulerAngles.z());
		
		Eigen::Matrix4f scaleMat;
		scaleMat.Identity();
		scaleMat(0,0) = scale.x();
		scaleMat(1,1) = scale.y();
		scaleMat(2,2) = scale.z();

		Eigen::Matrix4f shearMat;
		shearMat.Identity();
		if (shear.x() == 0.f) {
			shearMat(1,0) = shear.y();
			shearMat(2,0) = shear.z();
		}
		else if (shear.x() == 1.f) {
			shearMat(0,1) = shear.y();
			shearMat(2,1) = shear.z();
		}
		else if (shear.x() == 2.f) {
			shearMat(0,2) = shear.y();
			shearMat(1,2) = shear.z();
		}

		Eigen::Matrix4f reflectMat;
		reflectMat.Identity();
		reflectMat(0,0) = (reflect.x() == 1 ? -1.f : 1.f);
		reflectMat(1,1) = (reflect.y() == 1 ? -1.f : 1.f);
		reflectMat(2,2) = (reflect.z() == 1 ? -1.f : 1.f);
		
		Eigen::Matrix4f mat;
		// Order matters, so this multiplication not be correct.
		mat = translateMat * rotateXMat * rotateYMat * rotateZMat * scaleMat * shearMat * reflectMat;

		return mat;
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
		// Get the entitiy's transform component data.
		// position is simply the amount by which the object translates.
		position.x() = data["positionX"];
		position.y() = data["positionY"];
		position.z() = data["positionZ"];

		// eulerAngles are the angles in x, y and z by which the object rotates, in radians.
		eulerAngles.x() = data["angleX"];
		eulerAngles.y() = data["angleY"];
		eulerAngles.z() = data["angleZ"];

		// scale is the factor by which the object scales in the x, y and z axes.
		scale.x() = data["scaleX"];
		scale.y() = data["scaleY"];
		scale.z() = data["scaleZ"];

		// Shear is a but more complex. The first value indicates along which axes the object is being sheared.
		// The second two values are the how much it is being sheared by in the other two axes.
		// These other two axes change depending on the first.
		shear.x() = data["reflectX"];
		shear.y() = data["reflectY"];
		shear.z() = data["reflectZ"];

		// reflect is three Boolean values of whether the object reflects in the x, y or z axes.
		reflect.x() = data["reflectX"];
		reflect.y() = data["reflectY"];
		reflect.z() = data["reflectZ"];

		return true;
	}
}