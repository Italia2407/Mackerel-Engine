#pragma once
#include "CollisionData.h"
#include <functional>
#include <map>
#include "CollisionCallbackReceipt.h"

namespace MCK::Physics
{
	typedef std::function<void(CollisionData)> collisionCallback;
	/**
	 * Manages Collision Callbacks.
	 */
	class CollisionCallbackManager
	{
		std::map<unsigned int, collisionCallback> onCollisionCallbacks;
		unsigned int seed = 0;

		inline unsigned int  GenerateID() { return seed++; }

	public:
		/**
		 * Registers a collision callback function. This function will be called on invokation
		 * 
		 * \param function: The function to add to the callback
		 * \return: The callback receipt. This is required to unregister the function
		 */
		CollisionCallbackReceipt Register(collisionCallback function);

		/**
		 * Deregisters a callback and invalidates the receipt.
		 * 
		 * \param receipt: The callback receipt, generated when the callback is registered
		 */
		void Deregister(CollisionCallbackReceipt& receipt);

		/**
		 * Invoke all callbacks.
		 * 
		 * \param data The collision data to pass through to all callback recipients
		 */
		void Invoke(CollisionData data);
	};
}
