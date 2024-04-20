#pragma once

#include "Renderable.h"

#include <string>

struct SDL_Texture;

namespace fro
{
	template<typename TextType>
	concept StringAssignable = std::same_as<std::string, TextType> || std::convertible_to<TextType, const char*>;

	template<typename TextType>
	concept StringConvertible = requires(TextType text)
	{
		{ std::to_string(text) };
	};

	class Text final : public Renderable
	{
		fro_GENERATED_RENDERABLE_BODY(Text)

	public:
		template<typename TextType>
			requires StringAssignable<TextType> || StringConvertible<TextType>
		void setText(const TextType& text)
		{
			if constexpr (StringAssignable<TextType>)
				m_Text = text;
			else
				m_Text = std::to_string(text);

			updateTexture();
		}

		void setFont(const std::string& fontName, int fontSize);

	private:
		void updateTexture();

		SDL_Texture* m_pTexture{};

		std::string m_Text{};
		std::string m_FontName{};
		int m_FontSize{};
	};
}