#pragma once
#include <string>

namespace MCK::EntitySystem
{
	struct RegisteredComponentData
	{
		size_t componentSize;
		void (*resetFunction)(void*);
		std::string jsonKey;
	};
}
