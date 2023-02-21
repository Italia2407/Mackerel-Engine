#include "Entity.h"
#include "Component.h"

namespace MCK::EntitySystem
{
	/**
	 * The default constructor for this entity.
	 *
	 */
	Entity::Entity()
	{
		destroying = false;
		parent = nullptr; 
	}

	/**
	 * Adds a component to the entity.
	 *
	 * \param component: An instance of the component to add
	 */
	void Entity::AddComponent(Component* component)
	{
		components.push_back(component);
		component->entity = this;
	}

	/**
	 * Removes an instance of a component.
	 *
	 * \param Component: The instance of the component
	 */
	void Entity::RemoveComponent(Component* component)
	{
		int componentIndex;
		for (componentIndex = 0; componentIndex < components.size(); ++componentIndex)
		{
			if (components[componentIndex] == component)
			{
				components.erase(components.begin() + componentIndex);
				break;
			}
		}
	}

	/**
	 * Returns a component of a given type.
	 *
	 * \return A pointer to the requested component. Returns nullptr if none found.
	 */
	template<typename T> T* Entity::GetComponent()
	{
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			if (components[i]->IsType<T>())
			{
				return (T*)components[i];
			}
		}
	}

	/**
	 * Invoked when the entity is created.
	 * Invokes OnCreate on all attached components
	 */
	void Entity::OnCreate()
	{
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			components[i]->OnCreate();
		}
	}

	/**
	 * Invoked each frame. Updates all components.
	 *
	 */
	void Entity::OnUpdate()
	{

		for (unsigned int i = 0; i < components.size(); ++i)
		{
			components[i]->OnUpdate();
		}

	}

	/**
	 * Invoked at the end of each frame.
	 * Checks if the entity is queued to be destroyed and deallocates if so
	 *
	 */
	void Entity::OnFrameEnd()
	{
		if (destroying)
		{
			OnDestroy();
			Deallocate();
		}
	}

	/**
	 * Invoked before the entity is destroyed.
	 * Invokes on destroy on all componnets and child entities
	 *
	 */
	void Entity::OnDestroy()
	{
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			components[i]->OnDestroy();
		}

		for (unsigned int i = 0; i < childEntities.size(); ++i)
		{
			childEntities[i]->OnDestroy();
		}
	}

	/**
	 * Marks the entity for destruction.
	 *
	 */
	void Entity::Destroy()
	{
		destroying = true;
	}

	/**
	 * Deallocates the entity and all child entities from memory.
	 *
	 */
	void Entity::Deallocate()
	{
		// Recursively deallocate children
		for (unsigned int i = 0; i < childEntities.size(); ++i)
		{
			childEntities[i]->Deallocate();
		}

		// TODO deallocate (depends on how we allocate)

		for (unsigned int i = 0; i < components.size(); ++i)
		{
			// TODO deallocate components
		}
	}
}