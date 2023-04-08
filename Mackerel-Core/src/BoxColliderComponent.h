#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"

namespace MCK::EntitySystem
{
	class BoxColliderComponent : public Component
	{
	private:
		Eigen::Vector3f scale;  

	public:
		Eigen::Vector3f& Scale() { return scale; } 

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;

		TypeInfoRef GetType() override;
	};
}