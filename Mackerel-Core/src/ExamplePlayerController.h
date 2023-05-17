#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "InputComponent.h"
#include "CollisionData.h"

namespace MCK::ExamplePlayer
{
	class ExamplePlayerController : public EntitySystem::Component<ExamplePlayerController>
	{
	private:
		float moveForceMag = 40;
		float horizontalDamping =1.1f;
		float deceleration = 90; 
		float jumpVel = 8;

		EntitySystem::TransformComponent* transform;
		Physics::RigidbodyComponent* rigidbody;
		EntitySystem::InputComponent* input;

		std::function<void(MCK::Physics::CollisionData)> playerCollisionCallback;
		MCK::Physics::CollisionCallbackReceipt& receipt;

	public:

		void OnPlayerCollision(MCK::Physics::CollisionData data);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		bool Deserialise(json data) override;
	};
}
