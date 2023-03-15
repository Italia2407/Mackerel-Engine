#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Component.h"
#include "Scene.h"

using json = nlohmann::json;

namespace MCK::EntitySystem
{
	/**
	 * Creates a blank entity and adds it to the scene.
	 *
	 * \return The created entity
	 */
	Entity* Scene::CreateEntity()
	{
		Entity* newEntity = entityFactory.Get();

		newEntity->scene = this;

		entities.push_back(newEntity);

		return newEntity;
	}

	/**
	 * Frees an entity from memory. Deallocate/Destroy the entity prior to this
	 *
	 * \param entity The entity to be freed
	 */
	void Scene::FreeEntity(Entity* entity)
	{
		// If the entity has no parent, we need to remove it from the scenes list of entities
		if (entity->parent == nullptr)
		{
			size_t numEntities = entities.size();
			for (unsigned int i = 0; i < numEntities; ++i)
			{
				if (entities[i] == entity)
				{
					entities.erase(entities.begin() + i);
					break;
				}
			}
		}

		entityFactory.Recycle(entity);
	}

	/**
	 * Updates all a scene and all the entities in it.
	 * 
	 */
	void Scene::UpdateScene()
	{
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			entities[i]->UpdateEntity();
		}
	}

	/**
	 * Creates a component of a given type, specified by the key.
	 *
	 * \param key The key specifying the type of component
	 * \return A pointer to the component
	 */
	Component* Scene::CreateComponent(std::string key)
	{
		return componentFactory.CreateComponent(key);
	}

	/**
	 * Frees a component from memory.
	 *
	 * \param component The component to free
	 */
	void Scene::FreeComponent(Component* component)
	{
		componentFactory.FreeComponent(component);
	}

	/**
	 * Deserialises a scene from JSON.
	 *
	 * \param sceneJson The JSON object
	 */
	void Scene::Deserialise(json sceneJson)
	{
		json entitiesJson = sceneJson["scene"]["entities"];

		for (json& entityJson : entitiesJson)
		{
			Entity* newEntity = CreateEntity();
			newEntity->Deserialise(entityJson);
		}
	}
}