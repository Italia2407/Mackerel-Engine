#pragma once

#include "Component.h"
#include "Scene.h" 
#include "Input.h"
#include "UISystem.h"

#include <functional>

namespace MCK::EntitySystem
{
	class UIComponent : public Component
	{
	private:

		MCK::UI::UISystem uiSystem;
		std::vector<MCK::UI::UIElement*> UIElements;
		MCK::InputCallback buttonInputCallback;
		MCK::Input::InputSubReceipt receipt;
		bool paused;

	public:
		void CreateStandardHUD();
		void CreateStandardMenu(); 

		void CreateButton(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f, 
			const std::string& label = "Button", const ImVec2& size = ImVec2(100, 30), const ImVec4& labelColour = ImVec4(0, 0, 0, 1), const std::function<void()>& callback = nullptr, bool unpauseOnClick = true);

		void CreateShape(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f, 
			MCK::UI::ShapeElement::ShapeType type = MCK::UI::ShapeElement::ShapeType::Rectangle, const ImVec2& size = ImVec2(100, 100), const ImVec4& borderColour = ImVec4(1, 1, 1, 1), float borderThickness = 0.0f, ImTextureID imageID = nullptr);

		void CreateText(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f, 
			const std::string& text = "Text");

		void ToggleVisible();

		void PausedCallback(int32_t key, MCK::ButtonEvents ButtonEvents);

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;

		TypeInfoRef GetType() override;
	};
}