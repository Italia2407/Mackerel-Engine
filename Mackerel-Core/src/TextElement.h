#pragma once

#include "UIElement.h"

namespace MCK::UI
{
	class TextElement : public UIElement
	{
	private:
		std::string text;
		std::function<std::string()> updateFunction;

	public:
		TextElement();
		virtual ~TextElement();

		void SetText(const std::string& newText);
		const std::string& GetText() const;

		void SetUpdateFunction(const std::function<std::string()>& func);
		void UpdateText();

		virtual void Draw() override;
	};
}
