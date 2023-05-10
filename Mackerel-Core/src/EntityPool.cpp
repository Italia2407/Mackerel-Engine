#include "EntityPool.h"

#include "Entity.h"

namespace MCK::EntitySystem::Pooling
{
	EntityPool::EntityPool()
	{
		Allocate();
	}

	EntityPool::~EntityPool()
	{
		if (live.size() > 0)
		{
			// This is very bad!
			// Don't delete this while there are still live entities!!!
			// There could be more clean-up here, but in actual use cases this should
			//     probably cause a soft crash.
		}

		while (pool.size() > 0)
		{
			delete pool.top();
			pool.pop();
		}
	}

	Entity* EntityPool::Get()
	{
		Entity* ret = nullptr;

		if (pool.size() > 0)
			Allocate();

		live.emplace(pool.top());
		ret = pool.top();
		pool.pop();

		return ret;
	}

	bool EntityPool::Recycle(Entity* entity)
	{
		bool success = false;

		auto loc = live.find(entity);
		if (loc == live.end())
		{
			success = false;
		}
		else
		{
			// TODO: reinitialise/reset the data of the instance once this
			//           functionality is implemented in the entity class.
			pool.push(*loc); // push back to stack
			live.erase(loc); // remove from live set
		}

		return success;
	}

	void EntityPool::Recall()
	{
		for (auto inst : live)
		{
			inst->Destroy();
		}
	}

	void EntityPool::Allocate(uint32_t count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			Entity* newEntity = new Entity;
			pool.push(newEntity);
		}
	}
}