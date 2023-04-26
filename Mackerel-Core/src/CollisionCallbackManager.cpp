#include "CollisionCallbackManager.h"


namespace MCK::Physics
{

	CollisionCallbackReceipt CollisionCallbackManager::Register(collisionCallback func)
	{
		CollisionCallbackReceipt receipt;
		receipt.callbackID = GenerateID();
		receipt.registered = true;

		onCollisionCallbacks[receipt.callbackID] = func;

		return receipt;
	}

	void CollisionCallbackManager::Deregister(CollisionCallbackReceipt& receipt)
	{
		if (receipt.registered)
		{
			receipt.registered = false;

			auto iter = onCollisionCallbacks.find(receipt.callbackID);
			if (iter != onCollisionCallbacks.end())
				onCollisionCallbacks.erase(iter);
		}
	}

	void CollisionCallbackManager::Invoke(CollisionData data)
	{
		for (auto itt : onCollisionCallbacks)
		{
			itt.second(data);
		}
	}
}
