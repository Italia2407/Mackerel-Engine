#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Component.h"
#include "Scene.h"

using json = nlohmann::json;

namespace MCK::EntitySystem
{
	Entity* Scene::CreateEntity()
	{
		Entity* newEntity = entityFactory.Get();

		newEntity->scene = this;

		entities.push_back(newEntity);

		return newEntity;
	}

	
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

	Component* Scene::CreateComponent(std::string key)
	{
		return componentFactory.CreateComponent(key);
	}

	void Scene::FreeComponent(Component* component)
	{
		componentFactory.FreeComponent(component);
	}

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