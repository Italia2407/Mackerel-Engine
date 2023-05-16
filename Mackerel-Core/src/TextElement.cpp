#include "TextElement.h"

namespace MCK::UI
{
	TextElement::TextElement() : text("Text"), updateFunction(nullptr)
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

	void TextElement::SetUpdateFunction(const std::function<std::string()>& func) {
		updateFunction = func;
	}

	void TextElement::UpdateText() {
		if (updateFunction) {
			SetText(updateFunction());
		}
	}

	void TextElement::Draw()
	{
		UpdateText();
		if (IsVisible())
		{
			ImVec2 absPosition = GetAbsolutePosition();
			ImGui::SetCursorPos(absPosition);

			ImGui::SetWindowFontScale(GetScale());
			ImGui::TextColored(colour, "%s", text.c_str());
			ImGui::SetWindowFontScale(1.0f);  // reset the scale to normal
		}
	}
}
