#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CreateCollisionShapeInfo.h"

namespace MCK::Physics
{
	class CollisionComponent : public MCK::EntitySystem::Component
	{
	private:

		MCK::EntitySystem::TransformComponent* transform;
	public:
		btCollisionObject* collider = nullptr;
		btCollisionShape* collisionShape = nullptr;

		void UpdateColliderTransform();
		void SetCollisionShape(CreateCollisionShapeInfo shapeInfo);


		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		bool Deserialise(json data) override;

		TypeInfoRef GetType();
	};
}