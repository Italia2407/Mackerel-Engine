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

	void ButtonElement::SetTextureID(GLuint newTextureID)
	{
		textureID = newTextureID;
	}

	GLuint ButtonElement::GetTextureID() const
	{
		return textureID;
	}

	void ButtonElement::Draw()
	{
		if (IsVisible())
		{
			ImVec2 absPosition = GetAbsolutePosition();
			ImGui::SetCursorPos(absPosition);

			// Check if the button is pressed
			if (textureID != 0)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

				ImVec2 scaledSize = GetSize();
				if (ImGui::IsMouseHoveringRect(absPosition, ImVec2(absPosition.x + scaledSize.x, absPosition.y + scaledSize.y)))
				{
					scaledSize.x *= 1.1f; // Scale size by 10% when hovered
					scaledSize.y *= 1.1f;

					// Adjust position to scale from the center
					absPosition.x -= (scaledSize.x - GetSize().x) / 2;
					absPosition.y -= (scaledSize.y - GetSize().y) / 2;
					ImGui::SetCursorPos(absPosition);
				}

				if (ImGui::ImageButton((void*)(intptr_t)textureID, scaledSize))
				{
					// Call the onClickCallback if it is set
					if (onClickCallback)
					{
						onClickCallback();
					}
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();

			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(colour.x, colour.y, colour.z, GetTransparency()));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(labelColour.x, labelColour.y, labelColour.z, GetTransparency()));
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
}
