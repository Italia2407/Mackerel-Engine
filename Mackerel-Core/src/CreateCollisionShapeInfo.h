#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "ColliderTypes.h"

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
	};
}

