#include "UIComponent.h"

#include "Entity.h"

#include "Input.h"

#include "UISystem.h"

#include "TimeManager.h"

namespace MCK::EntitySystem
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef UIComponent::GetType()
	{
		return typeid(UIComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void UIComponent::OnCreate()
	{
		paused = true;
		MCK::TimeManager::setTimescale(0.0f);

		buttonInputCallback = std::bind(&UIComponent::PausedCallback, this, std::placeholders::_1, std::placeholders::_2); /* <- syntax for member functions */

		MCK::Input::Subscribe(MCK::Key::P, MCK::ButtonEvents::Pressed, buttonInputCallback, &receipt); 
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void UIComponent::OnUpdate()
	{
		// Loop through all the children and call Draw() for each of them
		for (MCK::UI::UIElement* element : UIElements)
		{
			element->Draw();
		}
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void UIComponent::OnDestroy()
	{
		UIElements.clear();

		//Unload Images

	}

	void UIComponent::PausedCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
	{
		if (ButtonEvents == MCK::ButtonEvents::Pressed)
		{
			if (key == MCK::Key::P)
			{
				if (paused)
					MCK::TimeManager::setTimescale(1);
				else
					MCK::TimeManager::setTimescale(0);
				ToggleVisible(); 
			}
		}
	}

	void UIComponent::ToggleVisible()
	{
		std::cout << "Button Pressed" << std::endl;
		paused = !paused;

		for (MCK::UI::UIElement* element : UIElements)
		{
			element->ToggleVisible();
		}
	}

	void UIComponent::CreateStandardHUD()
	{

		ImVec2 windowSize = ImVec2(1280, 720);//ImGui::GetWindowSize(); 

		// Define the position and size of the minimap
		ImVec2 minimapPosition = ImVec2(10, 10);
		ImVec2 minimapSize = ImVec2(200, 200);

		// Create a circle shape for the minimap
		CreateShape(false, minimapPosition, 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Circle, minimapSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f);

		// Define the position and size of the information boxes
		ImVec2 infoBoxSize = ImVec2(windowSize.x / 4, windowSize.y / 6);
		ImVec2 leftInfoBoxPosition = ImVec2(10, windowSize.y - infoBoxSize.y - 10);
		ImVec2 rightInfoBoxPosition = ImVec2(windowSize.x - infoBoxSize.x - 10, windowSize.y - infoBoxSize.y - 10);

		// Create a rectangle shape for the left information box
		CreateShape(false, leftInfoBoxPosition, 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, infoBoxSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f);

		// Create a rectangle shape for the right information box
		CreateShape(false, rightInfoBoxPosition, 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, infoBoxSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f);

		// Define the positions and texts for the text elements
		ImVec2 fpsTextPosition = ImVec2(10, leftInfoBoxPosition.y - 30);
		ImVec2 scoreTextPosition = ImVec2(windowSize.x - 150, rightInfoBoxPosition.y - 30);
		ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

		// Create a text element for the FPS counter
		CreateText(false, fpsTextPosition, 1.0f, textColor, 1.0f, "FPS: ");

		// Create a text element for the score
		CreateText(false, scoreTextPosition, 1.0f, textColor, 1.0f, "Score: ");
	}

	void UIComponent::CreateStandardMenu()
	{
		// Calculate the position of the menu to center it on the page

		ImVec2 windowSize = ImVec2(1280, 720);//ImGui::GetWindowSize(); 
		ImVec2 menuSize = ImVec2(400, 500);
		ImVec2 menuPosition = ImVec2((windowSize.x - menuSize.x) / 2, (windowSize.y - menuSize.y) / 2);

		// Create a background shape
		ImVec2 shapePosition = menuPosition;
		ImVec2 shapeSize = menuSize;
		CreateShape(true, shapePosition, 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::RoundedRectangle, shapeSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f);

		// Create a title text element
		ImVec2 titlePosition = ImVec2(menuPosition.x + (menuSize.x / 2) - 35, menuPosition.y + 30);
		CreateText(true, titlePosition, 1.5f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "Menu Title");

		// Create a button element
		ImVec2 buttonSize = ImVec2(120, 40);

		ImVec2 buttonPosition = ImVec2(menuPosition.x + (menuSize.x - buttonSize.x) / 2, menuPosition.y + (menuSize.y - buttonSize.y) / 2);
		CreateButton(true, buttonPosition, 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 1.0f), 1.0f, "Demo 1", buttonSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), []() {});

		// Create another button element
		ImVec2 buttonPosition2 = ImVec2(menuPosition.x + (menuSize.x - buttonSize.x) / 2, menuPosition.y + (menuSize.y - buttonSize.y) / 2 + 60);
		CreateButton(true, buttonPosition2, 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 1.0f), 1.0f, "Quit", buttonSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), []() {});
	}

	void UIComponent::CreateButton(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& label, const ImVec2& size, const ImVec4& labelColour, const std::function<void()>& callback, bool unpauseOnClick)
	{
		auto combinedCallback = [this, callback]() {
			if (callback) { 
				callback(); 
			}
			if (unpauseOnClick)
				ToggleVisible();
		};

		MCK::UI::UIElement* buttonElement = MCK::UI::UISystem::CreateButton(visible, position, scale, colour, transparency, label, size, labelColour, combinedCallback);

		UIElements.push_back(buttonElement);
	}

	void UIComponent::CreateShape(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, MCK::UI::ShapeElement::ShapeType type, const ImVec2& size, const ImVec4& borderColour, float borderThickness, ImTextureID imageID)
	{
		MCK::UI::UIElement* shapeElement = MCK::UI::UISystem::CreateShape(visible, position, scale, colour, transparency, type, size, borderColour, borderThickness, imageID);

		UIElements.push_back(shapeElement);
	}

	void UIComponent::CreateText(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& text)
	{
		MCK::UI::UIElement* shapeElement = MCK::UI::UISystem::CreateText(visible, position, scale, colour, transparency, text);

		UIElements.push_back(shapeElement);
	}

	bool UIComponent::Deserialise(json data)
	{
		return true;
	}
}