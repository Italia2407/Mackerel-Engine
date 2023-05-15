#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "InputComponent.h"

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
	public:

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		bool Deserialise(json data) override;
	};
}
