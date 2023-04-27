#pragma once

#include "UIElement.h"
#include <functional> 

namespace MCK::UI
{
	class ButtonElement : public UIElement
	{
	private:
		std::string label;
		ImVec2 size;
		ImVec4 labelColour;
		std::function<void()> onClickCallback;
		GLuint textureID;

	public:
		ButtonElement();
		virtual ~ButtonElement();

		void SetLabel(const std::string& newLabel);
		const std::string& GetLabel() const;

		void SetSize(const ImVec2& newSize);
		const ImVec2& GetSize() const;

		void SetLabelColour(const ImVec4& newLabelColour);
		const ImVec4& GetLabelColour() const;

		void SetOnClickCallback(const std::function<void()>& callback); 

		void SetTextureID(GLuint newTextureID);
		GLuint GetTextureID() const;

		virtual void Draw() override;
	};
}
