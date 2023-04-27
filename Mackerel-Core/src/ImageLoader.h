#pragma once

//#include "stb_image.h"
#include "imgui.h"
#include "Texture.h"

#include <string>

namespace MCK::UI
{
	class ImageLoader
	{
		public:
			void LoadFromFile(const std::string& filename);

	private:
		GLuint textureId;
	};
}
