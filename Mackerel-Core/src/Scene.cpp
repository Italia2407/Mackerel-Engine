#include "ComponentFactory.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Component.h"
#include "Scene.h"
#include "TimeManager.h"
#include "JsonHelpers.h"
#include "Assets.h"
#include "Renderer.h"

#include <iostream>

using json = nlohmann::json;

namespace MCK::EntitySystem
{
	void Scene::InitialiseScene()
	{
		if (!initialised)
		{
			physicsWorld.InitialiseWorld();

			// Initialize the audio engine
			audioEngine.Initialise();
		}


		// Load a sound with ID 1
		initialised = true;
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
		midFrame = true;
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
		//std::cout << TimeManager::getFPS() << std::endl;
		physicsWorld.ApplySimulation(static_cast<float>(delta));
		audioEngine.Update();

		midFrame = false;

		if (unloadQueued)
		{
			unloadQueued = false;
			Deallocate();
		}
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
		Entity* parentEntity = CreateEntity();

		for (json& entityJson : entitiesJson)
		{
			Entity* newEntity = CreateEntity();
			parentEntity->AddChild(newEntity);
			newEntity->Deserialise(entityJson["entity"]);
		}
	}

	void Scene::LoadScene(std::string path)
	{
		sceneLoaded = true;

		json sceneJson = Helpers::ParseJson(path);
		Deserialise(sceneJson);
	}

	void Scene::LoadSceneAdditive(std::string path)
	{
		sceneLoaded = true;

		json sceneJson = Helpers::ParseJson(path);
		Deserialise(sceneJson);
	}

	void Scene::UnloadScene()
	{
		if (midFrame)
			unloadQueued = true;
		else
			Deallocate();
	}

	void Scene::Deallocate()
	{
		initialised = false;
		audioEngine.Deallocate();
		sceneLoaded = false;
		physicsWorld.TeardownWorld();

		for (unsigned int i = unsigned int(entities.size()); i > 0; --i)
		{
			entities[i - 1]->Deallocate();
		}

		TextureLibrary::ReleaseLibrary();
		ShaderLibrary::ReleaseLibrary();
		MaterialLibrary::ReleaseLibrary();
		MeshLibrary::ReleaseLibrary();
		std::cout << "Here" << std::endl;
		Rendering::Renderer::ResetRenderer();
		std::cout << "Here 2" << std::endl;
	}

	Entity* Scene::FindEntityWithTag(std::string tag)
	{
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			Entity* e = FindEntityWithTag(tag, entities[i]);
			if (e != nullptr)
			{
				return e;
			}
		}

		return nullptr;
	}

	Entity* Scene::FindEntityWithTag(std::string tag, Entity* root)
	{
		if (root->HasTag(tag))
		{
			return root;
		}
		else if(root->childEntities.size() > 0)
		{
			for (unsigned int i = 0; i < root->childEntities.size(); ++i)
			{
				Entity* e = FindEntityWithTag(tag, root->childEntities[i]);
				
				if (e != nullptr)
				{
					return e;
				}
			}
		}

		return nullptr;
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