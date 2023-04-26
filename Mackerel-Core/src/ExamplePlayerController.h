#pragma once
#include "Component.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "InputComponent.h"

namespace MCK::ExamplePlayer
{
	class ExamplePlayerController : public EntitySystem::Component
	{
	private:
		float moveForceMag = 30;
		float horizontalDamping =1.f;
		float deceleration = 90;
		float jumpVel = 4;

		EntitySystem::TransformComponent* transform;
		Physics::RigidbodyComponent* rigidbody;
		EntitySystem::InputComponent* input;
	public:

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		TypeInfoRef GetType() override;
		bool Deserialise(json data) override;
	};
}
