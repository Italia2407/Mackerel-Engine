#include "ComponentFactory.h"
#include "TestDemoComponent.h"
#include "CoreComponentRegistrar.h"
using namespace MCK::EntitySystem;

#define REGISTER(comp, key) ComponentManager::RegisterComponent(key, sizeof(comp), &comp::Reset)

void RegisterComponents()
{
	MCK::RegisterCoreComponents();

	REGISTER(Demo::TestDemoComponent, "TestDemoComponent");
}

#undef REGISTER