#pragma once

#include "UIElement.h"

namespace MCK::UI
{
	class TextElement : public UIElement
	{
	private:
		std::string text;

	public:
		TextElement();
		virtual ~TextElement();

		void SetText(const std::string& newText);
		const std::string& GetText() const;

		virtual void Draw() override;
	};
}
