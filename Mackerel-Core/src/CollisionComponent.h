#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"
#include "TransformComponent.h"

namespace MCK::Physics
{
	class CollisionComponent : public MCK::EntitySystem::Component
	{
	private:

		MCK::EntitySystem::TransformComponent* transform;
	public:
		btCollisionObject* collider;
		btCollisionShape* collisionShape;

		void UpdateColliderTransform();

		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		bool Deserialise(json data) override;

		TypeInfoRef GetType();
	};
}