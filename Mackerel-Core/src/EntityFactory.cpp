#include "EntityFactory.h"

#include "Entity.h"

namespace MCK::EntitySystem
{
	Entity* EntityFactory::Get()
	{
		return pool.Get();
	}

	bool EntityFactory::Recycle(Entity* entity)
	{
		return pool.Recycle(entity);
	}

	void EntityFactory::Recall()
	{
		pool.Recall();
	}
}
