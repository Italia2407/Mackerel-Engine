#pragma once
#include <vector>
#include<iostream>

namespace MCK::EntitySystem 
{

	class Component;

	class Entity
	{
	public:
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
		std::vector<Component*> components;

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
		void OnUpdate();

		/**
		 * Invoked at the end of each frame. 
		 * Checks if the entity is queued to be destroyed and deallocates if so TODO make private
		 * 
		 */
		void OnFrameEnd();

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
		void AddComponent(Component* component);

		/**
		 * Returns a component of a given type.
		 * 
		 * \return A pointer to the requested component. Returns nullptr if none found.
		 */
		template<typename T> T* GetComponent();

		/**
		 * Removes an instance of a component.
		 * 
		 * \param Component: The instance of the component
		 */
		void RemoveComponent(Component* component);

		/*json TestEntityJson()
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
						}
					}}
				}}
			};

			return test;
		}

		void Deserialise(json entity)
		{
			// Test deserialisation
			std::cout << "Name: " << entity["name"] << std::endl;
			json comps = entity["components"];
			std::cout << "Component type: " << comps["type"] << std::endl;
		}*/

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