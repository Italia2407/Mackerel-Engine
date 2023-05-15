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
		MCK::AssetType::Mesh* mesh = nullptr;
	};
}

