#pragma once
#include <vector>

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
		 * Checks if the entity is queued to be destroyed and deallocates if so
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