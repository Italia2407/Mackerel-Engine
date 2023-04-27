#pragma once

#include <glad/glad.h>
#include "imgui.h"

#include <string>
#include <vector>

namespace MCK::UI
{
	/** A UIElement is an instance of an element to be drawn */
	class UIElement

	{
	protected:
		bool visible;
		ImVec2 position;
		float scale;
		ImVec4 colour;
		float transparency;
		UIElement* parent; 
		std::vector<UIElement*> children; 

	public:
		UIElement();
		virtual ~UIElement();

		void ToggleVisible();
		bool IsVisible() const;

		void SetPosition(const ImVec2& newPosition);
		const ImVec2& GetPosition() const;

		void SetScale(float newScale);
		float GetScale() const;

		void SetColour(const ImVec4& newColour);
		const ImVec4& GetColour() const;

		void SetTransparency(float newTransparency);
		float GetTransparency() const;

		void SetParent(UIElement* parent); 
		UIElement* GetParent() const; 

		void AddChild(UIElement* child); 
		void RemoveChild(UIElement* child); 
		const std::vector<UIElement*>& GetChildren() const; 

		ImVec2 GetAbsolutePosition() const;

		virtual void Draw() = 0;
	};
}