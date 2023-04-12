#pragma once

#include "App.h"

namespace MCK
{
	class AppAttorney
	{
		public:
			class App_Instance /* private App member functions exposed to the Instance class */
			{
				friend class Instance;

				static void BeforeLoop(App* app)
				{
					app->BeforeLoop();
				}

				static void DuringLoop(App* app)
				{
					app->DuringLoop();
				}

				static void AfterLoop(App* app)
				{
					app->AfterLoop();
				}
			};
	};
}
