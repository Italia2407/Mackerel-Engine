#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CreateCollisionShapeInfo.h"
#include "CollisionCallbackManager.h"

namespace MCK::Physics
{
	class CollisionComponent : public MCK::EntitySystem::Component<CollisionComponent>
	{
	private:

		MCK::EntitySystem::TransformComponent* transform;
	public:
		btCollisionObject* collider = nullptr;
		btCollisionShape* collisionShape = nullptr;
		CollisionCallbackManager onCollisionHandler;
		bool isTrigger = false;

		void UpdateColliderTransform();
		void SetCollisionShape(CreateCollisionShapeInfo shapeInfo);


		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		bool Deserialise(json data) override;
	};
}