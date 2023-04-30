#include "ComponentBase.h"

namespace MCK::EntitySystem
{
	void ComponentBase::UpdateComponent()
	{
		// If we're updating a disabled component, Invoke on create
		if (state == ComponentState::DISABLED)
		{
			// Set the state to Active - NB: Order important as it allows for state override in OnCreate
			state = ComponentState::ACTIVE;
			OnCreate();
		}


		// Update active components
		if (state == ComponentState::ACTIVE)
		{
			OnUpdate();
		}
	}
}
