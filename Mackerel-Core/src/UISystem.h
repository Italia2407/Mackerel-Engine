#pragma once

#include "imgui.h"
#include "../Dear-ImGui 1.89.2/backends/imgui_impl_glfw.h"
#include "../Dear-ImGui 1.89.2/backends/imgui_impl_opengl3.h"

#include "LoggingSystem.h"

#include "UIElement.h"
#include "TextElement.h"
#include "ButtonElement.h"
#include "ShapeElement.h"
#include "Context.h"
#include "Texture.h"

#include<map>
#include <functional>

namespace MCK::UI
{
	class UISystem
	{
		public:

			/**
			 * Initialise ImGui context
			 *
			 */
			static bool InitialiseImGui(MCK::Context* context);

			/**
			 * Draw main ImGui window
			 *
			 */
			static void NewFrame(MCK::Context* context);

			/**
			 * Initialise ImGui context
			 *
			 */
			static void CleanImGui();

			/**
			 * Creates a UIElement of type button
			 *
			 */
			static UIElement* CreateButton(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f,
				const std::string& label = "Button", const ImVec2& size = ImVec2(100, 30), const ImVec4& labelColour = ImVec4(0, 0, 0, 1), const std::function<void()>& callback = nullptr, MCK::AssetType::Texture* image = nullptr);

			/**
			 * Creates a UIElement of type shape
			 *
			 */
			static UIElement* CreateShape(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f,
				MCK::UI::ShapeElement::ShapeType type = MCK::UI::ShapeElement::ShapeType::Rectangle, const ImVec2& size = ImVec2(100, 100), const ImVec4& borderColour = ImVec4(1, 1, 1, 1), float borderThickness = 0.0f, MCK::AssetType::Texture* image = nullptr);

			/**
			 * Creates a UIElement of type text
			 *
			 */
			static UIElement* CreateText(bool visible = true, const ImVec2& position = ImVec2(0, 0), float scale = 1.0f, const ImVec4& colour = ImVec4(1, 1, 1, 1), float transparency = 1.0f,
				const std::string& text = "Text");

			/**
			 * Loads the requested image and stores it
			 *
			 */
			static void LoadUIImage(const char* filename, unsigned int imageID);

			/**
			 * Gets the requested image using its id
			 *
			 */
			static void GetImage(unsigned int imageID) ;

			/**
			 * Unloads the image, for teardown
			 *
			 */
			static void UnloadImage(unsigned int imageID);
	};

}