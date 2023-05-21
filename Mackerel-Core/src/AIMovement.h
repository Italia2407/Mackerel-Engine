#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "Component.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "SkinnedMeshRendererComponent.h"
#include "Entity.h"
#include "InputComponent.h"
#include "CollisionData.h"
#include "TimeManager.h"

namespace MCK::EntitySystem
{
	class AIMovement : public MCK::EntitySystem::Component<AIMovement>
	{
	private:
		MCK::EntitySystem::TransformComponent* transform;
		MCK::Physics::RigidbodyComponent* rigidbody;
		EntitySystem::SkinnedMeshRendererComponent* skinnedMesh;
		float moveForceMag = 5.0f;
		float reachThreshold;

		Eigen::Vector3f startPos;
		Eigen::Vector3f endPos;

		float horizontalDamping = 10.1f;
		float deceleration = 90;
		float turnRate = 10.0f;
		bool isReturning;
		Eigen::Vector3f smoothMoveVector{};

		Eigen::Vector2f calcInput();

	public:
		void OnCreate() override;
		void OnUpdate() override;
		bool Deserialise(json data) override;

		void setEndPosition(Eigen::Vector3f target);
	};
}

#endif // AICONTROLLER_H
