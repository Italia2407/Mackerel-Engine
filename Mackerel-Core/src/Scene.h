#pragma once

#include <vector>
#include "../ext/nlohmann/json.hpp"

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

	public:
		Entity* CreateEntity();
		void FreeEntity(Entity* entity);
		void Deserialise(json sceneJson);

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
		}
	};
}