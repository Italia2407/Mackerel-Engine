#pragma once
#include "Component.h"
#include "Scene.h"

#include "PerspectiveCamera.h"
#include "TimeManager.h"

#include "Eigen/Eigen.h"

namespace MCK::EntitySystem {
	class CameraFollowComponent : public Component<CameraFollowComponent>
	{
	private:
		PerspectiveCamera* camera;
		TransformComponent* transform;
		Entity* player;

		Eigen::Vector3f offset;
		Eigen::Vector3f position;
		float multiplier = 7.0f;

	public:

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;
	};
}
