#pragma once

#include <stack>
#include <set>

/**
 * The default number of entities to allocate if the pool is emptied and another
 *     entity is requested. Also doubles as the starting pool size.
 */
#define DEFAULT_ENTITY_ALLOC_SIZE 16U

namespace MCK::EntitySystem
{
	class Entity;
}

namespace MCK::EntitySystem::Pooling
{
	class EntityPool
	{
		public:
			EntityPool();
			~EntityPool();

			// no copy >:(
			EntityPool(const EntityPool&) = delete;
			EntityPool& operator=(const EntityPool&) = delete;

			/**
			 * The get function.
			 * Use in place of any 'new' calls for Entity objects.
			 * 
			 * \return a pointer to a valid (possibly recycled) entity instance
			 */
			Entity* Get();

			/**
			 * The recycle function.
			 * Use in place of any 'delete' calls for Entity instances retrieved from
			 *     this pool (that should be all of them!).
			 * 
			 * \param entity: the entity to be recycled
			 * 
			 * \return true if a valid entity was provided, false if not
			 */
			bool Recycle(Entity* entity);

			/**
			 * Forcibly destroys and recycles all of the live Entity instances.
			 * Use sparingly!
			 */
			void Recall();

			/**
			 * Allocates more Entity instances for the pool.
			 * 
			 * \param count: the count of new Entity instances to allocate.
			 */
			void Allocate(uint32_t count = DEFAULT_ENTITY_ALLOC_SIZE);

		private:
			/**
			 * The pool of ready-to-use, but inactive Entity instances.
			 */
			std::stack<Entity*> pool;

			/**
			 * The collection of Entity instances currently in use.
			 */
			std::set<Entity*> live;
	};
}