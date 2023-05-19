#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "InputComponent.h"
#include "CollisionData.h"
//#include "LoggingSystem.h"


namespace MCK::ExamplePlayer
{
	class ExamplePlayerController : public EntitySystem::Component<ExamplePlayerController>
	{
	private:
		float moveForceMag = 120;
		float horizontalDamping =10.1f;
		float deceleration = 90; 
		float jumpVel = 13;
		double lastGroundTime;

		EntitySystem::TransformComponent* transform;
		Physics::RigidbodyComponent* rigidbody;
		Eigen::Vector3f startPosition;
		EntitySystem::InputComponent* input;

		std::function<void(MCK::Physics::CollisionData)> playerCollisionCallback;
		MCK::Physics::CollisionCallbackReceipt receipt;

	public:

		void OnPlayerCollision(MCK::Physics::CollisionData data);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		bool Deserialise(json data) override;
	};
}
