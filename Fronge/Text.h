#pragma once

#include "Renderable.h"

#include <string>

namespace fro
{
	template<typename TextType>
	concept StringAssignable = std::same_as<std::string, TextType> || std::convertible_to<TextType, const char*>;

	template<typename TextType>
	concept StringConvertible = requires(TextType text)
	{
		{ std::to_string(text) };
	};
}

fro_GENERATED_RENDERABLE_BODY
(
	Text,

	public:
		template<typename TextType>
			requires StringAssignable<TextType> || StringConvertible<TextType>
		void setText(const TextType & text)
		{
			if constexpr (StringAssignable<TextType>)
				m_Text = text;
			else
				m_Text = std::to_string(text);
		}

		void setFont(const std::string& fileName, int size);

	private:
		std::string m_Text{};
		std::pair<std::string, int> m_Font{};
)