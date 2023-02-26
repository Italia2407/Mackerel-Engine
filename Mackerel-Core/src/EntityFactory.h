#pragma once

#include "EntityPool.h"

// Forward Declarations
namespace MCK::EntitySystem {
class Entity;
}

namespace MCK::EntitySystem
{
	class EntityFactory
	{
		public:
			EntityFactory() = default;
			~EntityFactory() = default;

			// there can be only one!
			EntityFactory(const EntityFactory&) = delete;
			EntityFactory& operator=(const EntityFactory&) = delete;
			
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

		private:
			Pooling::EntityPool pool;
	};
}