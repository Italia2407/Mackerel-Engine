#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Component.h"
#include "Scene.h"
#include "TimeManager.h"
#include "JsonHelpers.h"
#include "Assets.h"

#include <iostream>

using json = nlohmann::json;

namespace MCK::EntitySystem
{
	void Scene::InitialiseScene()
	{
		physicsWorld.InitialiseWorld();

		// Initialize the audio engine
		audioEngine.Initialise();

		// Load a sound with ID 1
		//MCK::Audio::Sound sound1 = audioEngine.LoadSound("../Mackerel-Core/res/Sounds/Voyager.mp3", 1, true, false, false);
	}

	/**
	 * Creates a blank entity and adds it to the scene.
	 *
	 * \return The created entity
	 */
	Entity* Scene::CreateEntity()
	{
		Entity* newEntity = entityFactory.Get();

		newEntity->scene = this;
		newEntity->id = GenerateEntityID();

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
		// Frame start
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			entities[i]->UpdateEntity();
		}

		// Frame end
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			entities[i]->FrameEnd();
		}

		double delta = MCK::TimeManager::getFrameTime();

		physicsWorld.ApplySimulation(static_cast<float>(delta));
		audioEngine.Update();
	}

	/**
	 * Creates a component of a given type, specified by the key.
	 *
	 * \param key The key specifying the type of component
	 * \return A pointer to the component
	 */
	ComponentBase* Scene::CreateComponent(std::string key)
	{
		return ComponentManager::AllocateComponent(key);
	}

	/**
	 * Frees a component from memory.
	 *
	 * \param component The component to free
	 */
	void Scene::FreeComponent(ComponentBase* component)
	{
		ComponentManager::DeallocateComponent(component);
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

	void Scene::LoadScene(std::string path)
	{
		if (sceneLoaded)
		{
			UnloadScene();
		}

		sceneLoaded = true;

		json sceneJson = Helpers::ParseJson(path);
		Deserialise(sceneJson);
	}

	void Scene::UnloadScene()
	{
		physicsWorld.TeardownWorld();

		for (unsigned int i = unsigned int(entities.size()); i > 0; --i)
		{
			entities[i-1]->Deallocate();
		}

		TextureLibrary::ReleaseLibrary();
		ShaderLibrary::ReleaseLibrary();
		MaterialLibrary::ReleaseLibrary();
		MeshLibrary::ReleaseLibrary();
	}

	/**
	 * Generates a unique ID for an entity.
	 * 
	 * \return The id
	 */
	entityId Scene::GenerateEntityID()
	{
		return idSeed++;
	}
}