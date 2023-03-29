#pragma once
#include "../ext/nlohmann/json.hpp"
#include "ComponentState.h"

// Forward Declacrations
namespace MCK::EntitySystem {
class Entity;
}

using json = nlohmann::json;
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

namespace MCK::EntitySystem 
{
	class Component
	{
	private:

		void UpdateComponent();
	public:
		
		/** A pointer to the entity that holds this component */
		Entity* entity;

		ComponentState state;

		/**
		 * Invoked when the entity holding the component is created.
		 * 
		 */
		virtual void OnCreate(){}

		/**
		 * Invoked when the component is updated each frame.
		 * 
		 */
		virtual void OnUpdate(){}

		/**
		 * Invoked before the entity holding this component is destroyed.
		 * 
		 */
		virtual void OnDestroy(){}

		/**
		 * Returns the type id of this component
		 * 
		 * \return The type info
		 */
		virtual TypeInfoRef GetType() = 0;

		/**
		 * Deserialises a component entity from a JSON object.
		 * 
		 * \param data: The JSON object.
		 * \return True if successful, false otherwise.
		 */
		virtual bool Deserialise(json data) = 0;

		/**
		 * Determines whether this component is of a specified type.
		 * 
		 * \return True if the component is of the specified type, false otherwise
		 */
		template<typename T> bool IsType() { return typeid(T) == GetType(); }
		
		friend class Entity;
	};
}