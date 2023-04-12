#pragma once

#include "Context.h"

namespace MCK
{
	class App
	{
		friend class AppAttorney;

		public:
			App();
			virtual ~App();

			App(const App&) = delete;
			App& operator=(const App&) = delete;

		protected:
			Context* context = nullptr;

			virtual void Init();
			virtual void Load();
			virtual void Start();
			virtual void Update();
			virtual void Draw();
			virtual void End();
			virtual void Unload();
			virtual void Release();

		private:
			void SetContext(Context* context);

			void BeforeLoop();
			void DuringLoop();
			void AfterLoop();
	};
}
