#pragma once

#include "App.h"

class Context;

namespace MCK
{
	class AppAttorney
	{
		public:
			class App_Instance /* private App member functions exposed to the Instance class */
			{
				friend class Instance;

				static void SetContext(App* app, Context* context)
				{
					app->SetContext(context);
				}

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
