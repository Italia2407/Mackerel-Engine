#include "TextElement.h"

namespace MCK::UI
{
	TextElement::TextElement() : text("Text") // White color
	{
	}


	TextElement::~TextElement()
	{

	}

	void TextElement::SetText(const std::string& newText)
	{
		text = newText;
	}

	const std::string& TextElement::GetText() const
	{
		return text;
	}

	void TextElement::Draw()
	{
		if (IsVisible())
		{
			ImVec2 absPosition = GetAbsolutePosition();
			ImGui::SetCursorPos(absPosition);
			ImGui::TextColored(colour, "%s", text.c_str());
		}
	}
}
