#pragma once

#include "UIElement.h"

namespace MCK::UI
{
	class ShapeElement : public UIElement
	{
	public:
		enum class ShapeType
		{
			Rectangle,
			Circle,
			RoundedRectangle
		};

	private:
		ShapeType shapeType;
		ImVec2 size;
		ImVec4 borderColour;
		float borderThickness;
		GLuint textureID;

	public:
		ShapeElement();
		virtual ~ShapeElement();

		void SetShapeType(ShapeType newShapeType);
		ShapeType GetShapeType() const;

		void SetSize(const ImVec2& newSize);
		const ImVec2& GetSize() const;

		void SetBorderColour(const ImVec4& newColour);
		const ImVec4& GetBorderColour() const;

		void SetBorderThickness(float newBorderThickness);
		float GetBorderThickness() const;

		void SetTextureID(GLuint newTextureID);
		GLuint GetTextureID() const;

		virtual void Draw() override;
	};
}
