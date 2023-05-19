#include "UIElement.h"

namespace MCK::UI
{
	UIElement::UIElement() : visible(true), position(ImVec2(0.0f, 0.0f)), scale(1.0f), colour(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
		transparency(1.0f), parent(nullptr)
	{

	}

	UIElement::~UIElement()
	{

	}

	void UIElement::SetVisible(bool isVisible)
	{
		visible = isVisible;
	}

	bool UIElement::IsVisible() const
	{
		return visible;
	}

	void UIElement::SetPosition(const ImVec2& newPosition)
	{
		position = newPosition;
	}

	const ImVec2& UIElement::GetPosition() const
	{
		return position;
	}

	void UIElement::SetScale(float newScale)
	{
		scale = newScale;
	}

	float UIElement::GetScale() const
	{
		return scale;
	}

	void UIElement::SetTransparency(float newTransparency)
	{
		transparency = newTransparency;
	}

	float UIElement::GetTransparency() const
	{
		return transparency;
	}

	void UIElement::SetColour(const ImVec4& newColour)
	{
		colour = newColour;
	}

	const ImVec4& UIElement::GetColour() const
	{
		return colour;
	}

	void UIElement::SetParent(UIElement* newParent)
	{
		parent = newParent;
	}

	UIElement* UIElement::GetParent() const
	{
		return parent;
	}

	void UIElement::AddChild(UIElement* child)
	{
		children.push_back(child);
		child->SetParent(this);
	}

	void UIElement::RemoveChild(UIElement* child)
	{
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
		{
			(*it)->SetParent(nullptr);
			children.erase(it);
		}
	}

	const std::vector<UIElement*>& UIElement::GetChildren() const
	{
		return children;
	}

	ImVec2 UIElement::GetAbsolutePosition() const
	{
		ImVec2 absPos = position;
		UIElement* currentParent = parent;

		while (currentParent)
		{
			ImVec2 parentPos = currentParent->GetPosition();
			absPos.x += parentPos.x;
			absPos.y += parentPos.y;
			currentParent = currentParent->GetParent();
		}

		return absPos;
	}

}
