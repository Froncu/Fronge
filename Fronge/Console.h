#pragma once

#include <iostream>
#include <windows.h>

namespace fro
{
	template<typename PrintableType>
	concept Printable = requires(std::ostream & outputStream, PrintableType value)
	{
		{ outputStream << value };
	};

	class Console final
	{
	public:
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

		static void setColor([[maybe_unused]] TextColor textColor, [[maybe_unused]] BackgroundColor backgroundColor)
		{
#ifdef _CONSOLE
			m_TEXT_COLOR = textColor;
			m_BACKGROUND_COLOR = backgroundColor;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<WORD>(m_TEXT_COLOR) | static_cast<WORD>(m_BACKGROUND_COLOR));
#endif
		}

		static void setColor([[maybe_unused]] TextColor textColor)
		{
#ifdef _CONSOLE
			setColor(textColor, m_BACKGROUND_COLOR);
#endif
		}

		static void setColor([[maybe_unused]] BackgroundColor backgroundColor)
		{
#ifdef _CONSOLE
			setColor(m_TEXT_COLOR, backgroundColor);
#endif
		}

		static void resetColor()
		{
#ifdef _CONSOLE
			setColor(TextColor::Gray, BackgroundColor::Black);
#endif
		}

		static void log([[maybe_unused]] const Printable auto& value)
		{
#ifdef _CONSOLE
			std::cout << value << std::endl;
#endif
		}

		static void log([[maybe_unused]] const Printable auto& value, [[maybe_unused]] TextColor textColor, [[maybe_unused]] BackgroundColor backgroundColor)
		{
#ifdef _CONSOLE
			const TextColor oldTextColor{ m_TEXT_COLOR };
			const BackgroundColor oldBackgroundColor{ m_BACKGROUND_COLOR };

			setColor(textColor, backgroundColor);
			log(value);
			setColor(oldTextColor, oldBackgroundColor);
#endif
		};

		static void log([[maybe_unused]] const Printable auto& value, [[maybe_unused]] TextColor textColor)
		{
#ifdef _CONSOLE
			log(value, textColor, m_BACKGROUND_COLOR);
#endif
		};

		static void log([[maybe_unused]] const Printable auto& value, [[maybe_unused]] BackgroundColor backgroundColor)
		{
#ifdef _CONSOLE
			log(value, m_TEXT_COLOR, backgroundColor);
#endif
		};

		static void clear()
		{
#ifdef _CONSOLE
			system("CLS");
#endif
		}

	private:
		Console() = delete;
		Console(const Console&) = delete;
		Console(Console&&) noexcept = delete;

		~Console() = delete;

		Console& operator=(const Console&) = delete;
		Console& operator=(Console&&) noexcept = delete;

#ifdef _CONSOLE
		static TextColor m_TEXT_COLOR;
		static BackgroundColor m_BACKGROUND_COLOR;
#endif
	};
}