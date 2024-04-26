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
	public:
		Text(const GameObject& parentingGameObject);

		virtual ~Text() override = default;

		virtual void render() const override;

		void setFont(const std::string& fontName, int fontSize);

		template<typename TextType>
			requires StringAssignable<TextType> || StringConvertible<TextType>
		void setText(const TextType & text)
		{
			if constexpr (StringAssignable<TextType>)
				m_Text = text;
			else
				m_Text = std::to_string(text);

			updateTexture();
		}

	private:
		Text(const Text&) = delete;
		Text(Text&&) noexcept = delete;

		Text& operator=(const Text&) = delete;
		Text& operator=(Text&&) noexcept = delete;

		void updateTexture();

		SDL_Texture* m_pTexture{};

		std::string m_Text{};
		std::string m_FontName{};
		int m_FontSize{};
	};
}