#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Component.h"
#include "Scene.h"

#include <cmath>


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
	void Entity::AddComponent(ComponentBase* component)
	{
		components.push_back(component);
		component->entity = this;
		component->state = ComponentState::DISABLED;
	}

	/**
	 * Serialises and adds a component from JSON.
	 *
	 * \param componentKey The key of the component to add
	 * \param data The data within the component
	 */
	void Entity::AddComponent(std::string componentKey, json data)
	{
		ComponentBase* component = scene->CreateComponent(componentKey);

		if (component != nullptr)
		{
			AddComponent(component);
			component->Deserialise(data);
		}
	}

	/**
	 * Removes an instance of a component.
	 *
	 * \param Component: The instance of the component
	 */
	void Entity::RemoveComponent(ComponentBase* component)
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
	 * Adds a tag to this entities set of tag.
	 *
	 * \param tag: The tag
	 */
	void Entity::AddTag(std::string tag)
	{
		tags.push_back(tag);
	}

	/**
	 * Removes a tag.
	 *
	 * \param tag: The tag
	 */
	void Entity::RemoveTag(std::string tag)
	{
		tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
	}

	/**
	 * Returns whether an entity has a given tag.
	 *
	 * \param tag: The tag
	 * \return: True if the entity has the tag, false otherwise
	 */
	bool Entity::HasTag(std::string tag)
	{
		auto it = std::find(tags.begin(), tags.end(), tag);
		return it != tags.end();
	}


	/**
	* Deserialises an entity from a JSON object, adding all listed components.
	*
	* \param entity: The entity in JSON
	*/
	void Entity::Deserialise(json entity)
	{
		// Get the entities components
		std::string j = entity.dump();
		json comps = entity["components"];

		for (int i = 0; i < comps.size(); ++i)
		{
			std::string t = comps[i]["type"].dump();
			AddComponent(comps[i]["type"].get<std::string>(), comps[i]);
		}

		json tags = entity["tags"];
		for (int i = 0; i < tags.size(); ++i)
		{
			AddTag(tags[i]);
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
			//components[i]->OnCreate();
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
		tags.clear();
	}

	void Entity::Compute()
	{
		// compute fibonacci
		
		int prev = 0;
		int current = 1;

		for (int i = 2; i <= 10; ++i)
		{
			int temp = current;
			current = prev + current;
			prev = temp;
		}
		std::cout << current << std::endl;
		

		/*
		double result = 0.0;

		for (int i = 0; i < 10000; ++i) {
			double temp = std::sin(std::exp(std::sqrt(static_cast<double>(i + 1))));
			result += temp;
		}

		std::cout << result << std::endl;
		*/
	}
}