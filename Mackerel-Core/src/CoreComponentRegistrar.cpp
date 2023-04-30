#include "CoreComponentRegistrar.h"

using namespace MCK::EntitySystem;
using namespace MCK::Physics;

namespace MCK
{
	#define REGISTER(comp, key) ComponentManager::RegisterComponent(key, sizeof(comp), &comp::Reset)

	void RegisterCoreComponents()
	{
		REGISTER(AudioEmitter, "AudioEmitter");
		REGISTER(AudioListener, "AudioListener");
		REGISTER(CollisionComponent, "CollisionComponent");
		REGISTER(ExamplePlayer::ExamplePlayerController, "ExamplePlayerController");
		REGISTER(InputComponent, "InputComponent");
		REGISTER(MeshRendererComponent, "MeshRendererComponent");
		REGISTER(OrthographicCamera, "OrthographicCamera");
		REGISTER(PerspectiveCamera, "PerspectiveCamera");
		REGISTER(RigidbodyComponent, "RigidbodyComponent");
		REGISTER(TransformComponent, "TransformComponent");
	}

	#undef REGISTER
}

