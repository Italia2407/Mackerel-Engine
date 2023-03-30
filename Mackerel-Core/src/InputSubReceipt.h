#pragma once

#include <functional>
#include <list>
#include <map>
#include <vector>

#include "Keys.h"
#include "ButtonEvents.h"

namespace MCK::Input
{
	using callbackFunc = std::function<void(int32_t, ButtonEvents)>;
	using callbackList = std::list<callbackFunc>;

	/**
	 * This struct is used to deregister groups of key event callback subscriptions,
	 *     pass it to subscribe function calls to append the relevant callbacks to
	 *     it, and pass it to unsubscribe function calls to unsubscribe all associated
	 *     callbacks.
	 */
	struct InputSubReceipt
	{
		// This is sort of bad form, but I don't want these to be accessable any
		//     where except the KeyHandler class.
		friend class ButtonHandler;

		private:
			struct IttTriplet
			{
				std::vector<callbackList::iterator> pressed_iterators;
				std::vector<callbackList::iterator> released_iterators;
				std::vector<callbackList::iterator> held_iterators;
			};
			std::map<int32_t, IttTriplet> data;

		public:
			bool ContainsKey(int32_t key)
			{
				return data.contains(key);
			}

			IttTriplet& GetItt(int32_t key)
			{
				return data[key];
			}
	};
}
