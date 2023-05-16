#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "ColliderTypes.h"
#include "Assets.h"

namespace MCK::Physics
{
	struct CreateCollisionShapeInfo
	{
		ColliderTypes colliderType;
		float radius;
		/** Size x */
		float width;
		/** Size y */
		float height;
		/** Size z */
		float depth;
		/** For mesh colliders, leave null and the collider will extract the mesh from the renderer */
		MCK::AssetType::Mesh* mesh = nullptr;
	};
}

