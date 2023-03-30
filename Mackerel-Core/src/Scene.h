#pragma once

#include <vector>
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "../ext/nlohmann/json.hpp"
#include "RigidbodyComponent.h"
#include "PhysicsWorld.h"

// Forward declarations
namespace MCK::EntitySystem {
	class Entity;
	class Component;
	class EntityFactory;
	class ComponentFactory;
}

using json = nlohmann::json;

namespace MCK::EntitySystem
{


	class Scene
	{
	private:
		std::vector<Entity*> entities;

		EntityFactory entityFactory;
		ComponentFactory componentFactory;

		entityId idSeed = 0;

		/**
		 * Generates a unique ID for an entity.
		 *
		 * \return The id
		 */
		entityId GenerateEntityID();
	public:

		/**
		 * Creates a blank entity and adds it to the scene.
		 * 
		 * \return The created entity
		 */
		Entity* CreateEntity();

		/**
		 * Frees an entity from memory. Deallocate/Destroy the entity prior to this
		 * 
		 * \param entity The entity to be freed
		 */
		void FreeEntity(Entity* entity);

		/**
		 * Updates all a scene and all the entities in it.
		 * 
		 */
		void UpdateScene();

		/**
		 * Deserialises a scene from JSON.
		 * 
		 * \param sceneJson The JSON object
		 */
		void Deserialise(json sceneJson);

		/**
		 * Creates a component of a given type, specified by the key.
		 * 
		 * \param key The key specifying the type of component
		 * \return A pointer to the component
		 */
		Component* CreateComponent(std::string key);

		/**
		 * Frees a component from memory.
		 * 
		 * \param component The component to free
		 */
		void FreeComponent(Component* component);

		/**
		 * Test JSON for a scene. TODO remove
		 * 
		 * \return 
		 */
		json TestSceneJson()
		{
			json entity1 = {
				{"entity",{
					{"name","entity-name"},
					{"components",{
						{
							{"type","TestComponent"},
							{"data", {
								{"exampleAttribute1","entity1String"},
								{"exampleAttribute2",42}
							}}
						}
					}}
				}}
			};
		

			json entity2 = {
				{"entity",{
					{"name","entity-name"},
					{"components",{
						{
							{"type","TestComponent"},
							{"data", {
								{"exampleAttribute1","entity2String"},
								{"exampleAttribute2",85}
							}}
						}
					}}
				}}
			};

			json scene = {
				{"scene",{
					{"exampleAttribute","some scene attribute"},
					{"entities",{
						entity1,
						entity2
					}}

				}}
			};
		
			return scene;
		}

		};
}