#pragma once
#include "Component.h"
#include "Scene.h"

#include "PerspectiveCamera.h"
#include "TimeManager.h"

#include "Eigen/Eigen.h"

namespace MCK::EntitySystem {
	class CameraFollowComponent : public ComponentBase
	{
	private:
		PerspectiveCamera* camera;
		TransformComponent* transform;
		Entity* player;

		Eigen::Vector3f offset;
		float x;

	public:

		void OnCreate() override;
		void OnUpdate() override;
		//void OnDestroy() override;
		//bool Deserialise(json data) override;
	};
}
