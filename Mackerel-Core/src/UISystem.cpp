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

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);

		// Initialize ImGui bindings for GLFW and OpenGL
		ImGui_ImplGlfw_InitForOpenGL(**context, true);
		const char* glsl_version = "#version 130";
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
	UIElement* UISystem::CreateButton(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& label, const ImVec2& size, const ImVec4& labelColour, const std::function<void()>& callback, MCK::AssetType::Texture* image)
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
		if (image != nullptr)
			buttonElement->SetTextureID(image->getTextureID());

		return buttonElement;
	}

	/**
	 * Creates a UIElement of type text
	 *
	 */
	UIElement* UISystem::CreateText(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, const std::string& text, const std::function<std::string()>& updateFunc)
	{
		TextElement* textElement = new TextElement();
		if (!visible)
			textElement->ToggleVisible();
		textElement->SetPosition(position);
		textElement->SetScale(scale);
		textElement->SetColour(colour);
		textElement->SetTransparency(transparency);
		textElement->SetText(text);
		textElement->SetUpdateFunction(updateFunc);

		return textElement;
	}

	/**
	 * Creates a UIElement of type shape
	 *
	 */
	UIElement* UISystem::CreateShape(bool visible, const ImVec2& position, float scale, const ImVec4& colour, float transparency, MCK::UI::ShapeElement::ShapeType type, const ImVec2& size, const ImVec4& borderColour, float borderThickness, MCK::AssetType::Texture* image)
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
		if (image != nullptr)
			shapeElement->SetTextureID(image->getTextureID());

		return shapeElement;
	}
}
