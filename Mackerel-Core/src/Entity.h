#pragma once


#include <vector>
#include <iostream>
#include <string>
#include "../ext/nlohmann/json.hpp"

#include "Component.h"

typedef long unsigned int entityId;

// Forward Declarations
namespace MCK::EntitySystem {
class Scene;
class ComponentFactory;
}

using json = nlohmann::json;

namespace MCK::EntitySystem 
{
class Entity
{
private:
public:
	entityId id;

	Scene* scene;

	/**
		* The parent entity. Nullptr if non-existent.
		*/
	Entity* parent;

	/**
		* The children of this entity.
		*/
	std::vector<Entity*> childEntities;

	/**
		* The components attached to this entity.
		*/
	std::vector<ComponentBase*> components;

	/**
		* The default constructor for this entity.
		* 
		*/
	Entity();

	/**
		* Invoked when the entity is created.
		* Invokes OnCreate on all attached components
		*/
	void OnCreate();

	/**
		* Invoked each frame. Updates all components.
		* 
		*/
	void UpdateEntity();

	/**
		* Invoked at the end of each frame. 
		* Checks if the entity is queued to be destroyed and deallocates if so TODO make private
		* 
		*/
	void FrameEnd();

	/**
		* Invoked before the entity is destroyed. 
		* Invokes on destroy on all componnets and child entities
		* 
		*/
	void OnDestroy();

	/**
		* Adds a component to the entity.
		* 
		* \param component: An instance of the component to add
		*/
	void AddComponent(ComponentBase* component);

	/**
		* Serialises and adds a component from JSON.
		*
		* \param componentKey The key of the component to add
		* \param data The data within the component
		*/
	void AddComponent(std::string componentKey, json data);

	/**
	 * Returns a component of a given type.
	 *
	 * \return A pointer to the requested component. Returns nullptr if none found.
	 */
	template<typename T> T* GetComponent()
	{
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			if (components[i]->IsType<T>())
			{
				return (T*)components[i];
			}
		}

		return nullptr;
	}

	/**
		* Removes an instance of a component.
		* 
		* \param Component: The instance of the component
		*/
	void RemoveComponent(ComponentBase* component);

	/**
		* Test function. TODO remove
		* 
		* \return 
		*/
	json TestEntityJson()
	{
		json test = {
			{"entity",{
				{"name","entity-name"},
				{"components",{
					{
						{"type","TestComponent"},
						{"data", {
							{"exampleAttribute1","exampleString"},
							{"exampleAttribute2",42}
						}}
					},
					{
						{"type","TestComponent2"},
						{"data", {
							{"exampleAttribute3","exampleString2"},
							{"exampleAttribute4",100}
						}}
					}
				}}
			}}
		};

		json test2 = {
			{"entity",{
				{"name","entity-name"},
				{"components",{
					{
						{"type","TestComponent"},
						{"data", {
							{"exampleAttribute1","exampleString"},
							{"exampleAttribute2",42}
						}}
					},
					{
						{"type","TestComponent2"},
						{"data", {
							{"exampleAttribute3","exampleString2"},
							{"exampleAttribute4",100}
						}}
					}
				}},
				{"children",{
					test
				}}
			}}
		};

		return test;
	}

	/**
		* Deserialises an entity from a JSON object, adding all listed components.
		* 
		* \param entity: The entity in JSON
		*/
	void Deserialise(json entity);


	/**
		* Marks the entity for destruction.
		* 
		*/
	void Destroy();

	/**
		* Deallocates the entity and all child entities from memory.
		* 
		*/
	void Deallocate();

private:
	/**
		* Whether this entity is marked for destruction.
		*/
	bool destroying;

};
}