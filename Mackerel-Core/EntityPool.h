#pragma once

#include <queue>

namespace MCK::EntitySystem
{
	class Entity;

	class EntityPool
	{
		public:
			EntityPool();
			~EntityPool();

			// no copy >:(
			EntityPool(const EntityPool&) = delete;
			EntityPool& operator=(const EntityPool&) = delete;

			/**
			*/
			Entity* Get();
			void Recycle(Entity* entity);

		private:
			std::queue<Entity*> pool;
	};
}