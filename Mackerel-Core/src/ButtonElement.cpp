#include "ButtonElement.h"

namespace MCK::UI
{
	ButtonElement::ButtonElement() : label("Button"), size(ImVec2(100, 30)), labelColour(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) // Default size
	{

	}


	ButtonElement::~ButtonElement()
	{

	}

	void ButtonElement::SetLabel(const std::string& newLabel)
	{
		label = newLabel;
	}

	const std::string& ButtonElement::GetLabel() const
	{
		return label;
	}

	void ButtonElement::SetSize(const ImVec2& newSize)
	{
		size = newSize;
	}

	const ImVec2& ButtonElement::GetSize() const
	{
		return size;
	}

	void ButtonElement::SetLabelColour(const ImVec4& newLabelColour)
	{
		labelColour = newLabelColour;
	}

	const ImVec4& ButtonElement::GetLabelColour() const
	{
		return labelColour;
	}

	void ButtonElement::SetOnClickCallback(const std::function<void()>& callback)
	{
		onClickCallback = callback;
	}

	void ButtonElement::Draw()
	{
		if (IsVisible())
		{
			ImVec2 absPosition = GetAbsolutePosition();
			ImGui::SetCursorPos(absPosition);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(colour.x, colour.y, colour.z, GetTransparency()));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(labelColour.x, labelColour.y, labelColour.z, GetTransparency()));

			// Check if the button is pressed
			if (ImGui::Button(GetLabel().c_str(), GetSize()))
			{
				// Call the onClickCallback if it is set
				if (onClickCallback)
				{
					onClickCallback();
				}
			}

			ImGui::PopStyleColor(); 
			ImGui::PopStyleColor();
		}
	}
}
