#include "ShapeElement.h"

namespace MCK::UI
{
	ShapeElement::ShapeElement() : shapeType(ShapeType::Rectangle), size(ImVec2(100, 100)), // Default size
		borderColour(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), borderThickness(0.0f), textureID(0)
	{

	}

	ShapeElement::~ShapeElement()
	{

	}

	void ShapeElement::SetShapeType(ShapeType newShapeType)
	{
		shapeType = newShapeType;
	}

	ShapeElement::ShapeType ShapeElement::GetShapeType() const
	{
		return shapeType;
	}

	void ShapeElement::SetSize(const ImVec2& newSize)
	{
		size = newSize;
	}

	const ImVec2& ShapeElement::GetSize() const
	{
		return size;
	}

	void ShapeElement::SetBorderColour(const ImVec4& newColour)
	{
		borderColour = newColour;
	}

	const ImVec4& ShapeElement::GetBorderColour() const
	{
		return borderColour;
	}

	void ShapeElement::SetBorderThickness(float newBorderThickness)
	{
		borderThickness = newBorderThickness;
	}

	float ShapeElement::GetBorderThickness() const
	{
		return borderThickness;
	}

	void ShapeElement::SetTextureID(GLuint newTextureID)
	{
		textureID = newTextureID;
	}

	GLuint ShapeElement::GetTextureID() const
	{
		return textureID;
	}

	void ShapeElement::Draw()
	{
		if (IsVisible())
		{
			ImVec4 currentColour = GetColour();
			ImVec4 currentBorderColour = GetBorderColour();
			currentColour.w = GetTransparency();
			currentBorderColour.w = GetTransparency();

			ImVec2 absPosition = GetAbsolutePosition();
			ImGui::SetCursorPos(absPosition);

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			switch (shapeType)
			{
			case ShapeType::Rectangle:
				if (textureID != 0)
				{
					drawList->AddImage((void*)(intptr_t)textureID, position, ImVec2(absPosition.x + size.x, absPosition.y + size.y), ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
				}
				else
				{
					drawList->AddRectFilled(position, ImVec2(absPosition.x + size.x, absPosition.y + size.y), ImGui::ColorConvertFloat4ToU32(currentColour));
					drawList->AddRect(position, ImVec2(absPosition.x + size.x, absPosition.y + size.y), ImGui::ColorConvertFloat4ToU32(currentBorderColour), 0, ImDrawCornerFlags_All, borderThickness);
				}
				break;

			case ShapeType::Circle:
				drawList->AddCircleFilled(ImVec2(absPosition.x + size.x / 2, absPosition.y + size.y / 2), size.x / 2, ImGui::ColorConvertFloat4ToU32(currentColour));
				drawList->AddCircle(ImVec2(absPosition.x + size.x / 2, absPosition.y + size.y / 2), size.x / 2, ImGui::ColorConvertFloat4ToU32(currentBorderColour), 0, borderThickness);
				break;

			case ShapeType::RoundedRectangle:
				drawList->AddRectFilled(position, ImVec2(absPosition.x + size.x, absPosition.y + size.y), ImGui::ColorConvertFloat4ToU32(currentColour), 8.0f, ImDrawCornerFlags_All);
				drawList->AddRect(position, ImVec2(absPosition.x + size.x, absPosition.y + size.y), ImGui::ColorConvertFloat4ToU32(currentBorderColour), 8.0f, ImDrawCornerFlags_All, borderThickness);
				break;

			default:
				break;
			}

		}
	}
}

