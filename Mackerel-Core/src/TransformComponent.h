#pragma once

#include "Eigen/Core.h"
#include "Component.h"

namespace MCK::EntitySystem
{
	class TransformComponent : public Component
	{
	public:
		Eigen::Vector3f position;
		Eigen::Vector3f eulerAngles;
		Eigen::Vector3f scale;

		Eigen::Matrix4f GetTransformationMatrix();

		void OnCreate();
		void OnUpdate();
		void OnDestroy();
		bool Deserialise(json data);

		TypeInfoRef GetType();
	};
}