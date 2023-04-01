#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"


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
	 * Serialises and adds a component from JSON.
	 *
	 * \param componentKey The key of the component to add
	 * \param data The data within the component
	 */
	void Entity::AddComponent(std::string componentKey, json data)
	{
		Component* component = scene->CreateComponent(componentKey);

		if (component != nullptr)
		{
			component->Deserialise(data);

			AddComponent(component);
		}
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
	* Deserialises an entity from a JSON object, adding all listed components.
	*
	* \param entity: The entity in JSON
	*/
	void Entity::Deserialise(json entity)
	{
		// Get the entities components
		json comps = entity["entity"]["components"];

		for (int i = 0; i < comps.size(); ++i)
		{
			AddComponent(comps[i]["type"].get<std::string>(), comps[i]["data"]);
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
	void Entity::UpdateEntity()
	{

		for (unsigned int i = 0; i < components.size(); ++i)
		{
			components[i]->UpdateComponent();
		}

		for (unsigned int i = 0; i < childEntities.size(); ++i)
		{
			childEntities[i]->UpdateEntity();
		}

	}

	/**
	 * Invoked at the end of each frame.
	 * Checks if the entity is queued to be destroyed and deallocates if so
	 *
	 */
	void Entity::FrameEnd()
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

		for (unsigned int i = 0; i < components.size(); ++i)
		{
			scene->FreeComponent(components[i]);
		}

		childEntities.clear();
		components.clear();
		scene->FreeEntity(this);
	}
}