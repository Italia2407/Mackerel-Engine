#pragma once

namespace MCK::EntitySystem
{ 
	
	enum ComponentState
	{
		ACTIVE, // Component in use
		INACTIVE, // Component in use but temporarily suspended
		DISABLED // Component not in use
	};
}
