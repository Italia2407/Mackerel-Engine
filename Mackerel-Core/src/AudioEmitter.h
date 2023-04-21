#pragma once
#include "Component.h"

namespace MCK::EntitySystem
{
	class TransformComponent : public Component
	{
	private:
		unsigned int emitterSeed = 0;
		unsigned int emitterID = 0;
		unsigned int currentChannelID = 0;

		std::string soundFileName = "";
		bool soundLoop = false;
		bool sound3D = true;

	public:
		//void OnCreate() override;
		//void OnUpdate() override;
		//void OnDestroy() override;
		//bool Deserialise(json data) override;

		//TypeInfoRef GetType() override;
	};
}