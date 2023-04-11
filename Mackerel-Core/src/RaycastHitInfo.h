#pragma once

#include "../bullet3-master/src/btBulletCollisionCommon.h"

namespace MCK::Physics
{
	struct RaycastHit
	{
		// reference to hit entity
		MCK::EntitySystem::Entity* hitEntity;

		// some information
		btVector3 point;
		btVector3 normal;
		btVector3 distance;

		// if something has been hit
		bool hit;
	};
}
