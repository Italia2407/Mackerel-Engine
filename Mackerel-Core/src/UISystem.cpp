#include "UISystem.h"

namespace MCK::UI
{
	/**
	* Initialise ImGui context
	*
	*/
	bool UISystem::InitialiseImGui(MCK::Context* context)
	{
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

		// Initialize ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.0f; // Set alpha to 0.0f for full transparency
		style.Colors[ImGuiCol_TitleBg].w = 0.0f; // Set alpha to 0.0f for full transparency
		style.Colors[ImGuiCol_TitleBgActive].w = 0.0f; // Set alpha to 0.0f for full transparency		

		// Initialize ImGui bindings for GLFW and OpenGL
		ImGui_ImplGlfw_InitForOpenGL(**context, true);
		const char* glsl_version = "#version 130"; // Use appropriate GLSL version string depending on your OpenGL version
		ImGui_ImplOpenGL3_Init(glsl_version);

		return true;
	}

	/**
	* Draw main ImGui window
	*
	*/
	void UISystem::NewFrame(MCK::Context* context)
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(context->Width()), static_cast<float>(context->Height())));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("Main Window", nullptr, window_flags);
	}

	/**
	* Clean ImGui context
	*
	*/
	void UISystem::CleanImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	/**
	 * Creates a UIElement of type button
	 *
	 */
	UIElement* UISystem::CreateButton(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& label, const ImVec2& size, const ImVec4& labelColour, const std::function<void()>& callback)
	{
		ButtonElement* buttonElement = new ButtonElement;

		if (!visible)
			buttonElement->ToggleVisible();
		buttonElement->SetPosition(position);
		buttonElement->SetScale(scale);
		buttonElement->SetColour(colour);
		buttonElement->SetTransparency(transparency);
		buttonElement->SetLabel(label);
		buttonElement->SetSize(size);
		buttonElement->SetLabelColour(labelColour);
		buttonElement->SetOnClickCallback(callback); 

		return buttonElement;
	}

	/**
	 * Creates a UIElement of type text
	 *
	 */
	UIElement* UISystem::CreateText(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& text)
	{
		TextElement* textElement = new TextElement();
		if (!visible)
			textElement->ToggleVisible();
		textElement->SetPosition(position);
		textElement->SetScale(scale);
		textElement->SetColour(colour);
		textElement->SetTransparency(transparency);
		textElement->SetText(text);

		return textElement;
	}

	/**
	 * Creates a UIElement of type shape
	 *
	 */
	UIElement* UISystem::CreateShape(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, MCK::UI::ShapeElement::ShapeType type, const ImVec2& size, const ImVec4& borderColour, float borderThickness, ImTextureID imageID)
	{
		ShapeElement* shapeElement = new ShapeElement();
		if (!visible)
			shapeElement->ToggleVisible();
		shapeElement->SetPosition(position);
		shapeElement->SetScale(scale);
		shapeElement->SetColour(colour);
		shapeElement->SetTransparency(transparency);
		shapeElement->SetShapeType(type);
		shapeElement->SetSize(size);
		shapeElement->SetBorderColour(borderColour);
		shapeElement->SetBorderThickness(borderThickness);
		//shapeElement->SetImageID(imageID);

		return shapeElement;
	}


	void UISystem::LoadUIImage(const char* filename, unsigned int imageID)
	{
		//auto found = loadedImages.find(imageID);
		//if (found != loadedImages.end())
			//return found->second;

		//Image* newImage = nullptr;
		//auto result = imageLoader->createImage(filename, nullptr, &newImage);

		//if (result == True && newImage)
		//{
			//Image image = { imageID, newImage };
			//loadedImages[imageID] = image;
			//return image;
		//}
		//else
		//{
			//std::ostringstream output; 
			//output << "ImageID: " << imageID << " could not be loaded"; 
			//std::string outputStr = output.str();

			//MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Error, std::source_location::current()); 

		//Image emptyImage = { 0 };
		//return emptyImage;
		//}
	}

	void UISystem::GetImage(unsigned int imageID)
	{
		//return 0;
	}

	void UISystem::UnloadImage(unsigned int imageID)
	{
		// Unload the image

	}
}
