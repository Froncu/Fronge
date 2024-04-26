#pragma once

#include "Singleton.hpp"

#include <iostream>
#include <windows.h>

namespace fro
{
	template<typename PrintableType>
	concept Printable = requires(std::ostream & outputStream, PrintableType value)
	{
		{ outputStream << value };
	};

	class Console final : public Singleton<Console>
	{
	public:
		Console() = default;

		virtual ~Console() override = default;

		enum class TextColor
		{
			Black = 0x00,
			DarkerBlue = 0x01,
			DarkGreen = 0x02,
			LightBlue = 0x03,
			DarkRed = 0x04,
			Purple = 0x05,
			Orange = 0x06,
			Gray = 0x07,
			DarkGray = 0x08,
			DarkBlue = 0x09,
			Green = 0x0a,
			LighterBlue = 0x0b,
			Red = 0x0c,
			Pink = 0x0d,
			Yellow = 0x0e,
			White = 0x0f
		};

		enum class BackgroundColor
		{
			Black = 0x00,
			DarkerBlue = 0x10,
			DarkGreen = 0x20,
			LightBlue = 0x30,
			DarkRed = 0x40,
			Purple = 0x50,
			Orange = 0x60,
			Gray = 0x70,
			DarkGray = 0x80,
			DarkBlue = 0x90,
			Green = 0xa0,
			LighterBlue = 0xb0,
			Red = 0xc0,
			Pink = 0xd0,
			Yellow = 0xe0,
			White = 0xf0
		};

		void setColor([[maybe_unused]] TextColor const textColor, [[maybe_unused]] BackgroundColor const backgroundColor)
		{
#ifdef _CONSOLE
			m_TextColor = textColor;
			m_BackgroundColor = backgroundColor;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(m_TextColor) | static_cast<WORD>(m_BackgroundColor));
#endif
		}

		void setColor([[maybe_unused]] TextColor const textColor)
		{
#ifdef _CONSOLE
			setColor(textColor, m_BackgroundColor);
#endif
		}

		void setColor([[maybe_unused]] BackgroundColor const backgroundColor)
		{
#ifdef _CONSOLE
			setColor(m_TextColor, backgroundColor);
#endif
		}

		void resetColor()
		{
#ifdef _CONSOLE
			setColor(TextColor::Gray, BackgroundColor::Black);
#endif
		}

		void log([[maybe_unused]] Printable auto const& value) const
		{
#ifdef _CONSOLE
			std::cout << value << std::endl;
#endif
		}

		void log([[maybe_unused]] Printable auto const& value, [[maybe_unused]] TextColor const textColor, [[maybe_unused]] BackgroundColor const backgroundColor)
		{
#ifdef _CONSOLE
			TextColor const oldTextColor{ m_TextColor };
			BackgroundColor const oldBackgroundColor{ m_BackgroundColor };

			setColor(textColor, backgroundColor);
			log(value);
			setColor(oldTextColor, oldBackgroundColor);
#endif
		};

		void log([[maybe_unused]] Printable auto const& value, [[maybe_unused]] TextColor const textColor)
		{
#ifdef _CONSOLE
			log(value, textColor, m_BackgroundColor);
#endif
		};

		void log([[maybe_unused]] Printable auto const& value, [[maybe_unused]] BackgroundColor const backgroundColor)
		{
#ifdef _CONSOLE
			log(value, m_TextColor, backgroundColor);
#endif
		};

		void clear() const
		{
#ifdef _CONSOLE
			system("CLS");
#endif
		}

	private:
		Console(Console const&) = delete;
		Console(Console&&) noexcept = delete;

		Console& operator=(Console const&) = delete;
		Console& operator=(Console&&) noexcept = delete;

#ifdef _CONSOLE
		TextColor m_TextColor{ TextColor::Gray };
		BackgroundColor m_BackgroundColor{ BackgroundColor::Black };
#endif
	};
}