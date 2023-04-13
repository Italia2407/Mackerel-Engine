#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"

namespace MCK::EntitySystem
{
	class InputComponent : public Component
	{
	private:
		Eigen::Vector2f direction;  

	public:
		inline Eigen::Vector2f& Direction() { return direction; }

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;

		TypeInfoRef GetType() override;
	};
}