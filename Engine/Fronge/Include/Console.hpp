#if not defined fro_CONSOLE_H
#define fro_CONSOLE_H

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
		friend class Singleton<Console>;

	public:
		virtual ~Console() override = default;

		enum class TextColor
		{
			black = 0x00,
			darkerBlue = 0x01,
			darkGreen = 0x02,
			lightBlue = 0x03,
			darkRed = 0x04,
			purple = 0x05,
			orange = 0x06,
			gray = 0x07,
			darkGray = 0x08,
			darkBlue = 0x09,
			green = 0x0a,
			lighterBlue = 0x0b,
			red = 0x0c,
			pink = 0x0d,
			yellow = 0x0e,
			white = 0x0f
		};

		enum class BackgroundColor
		{
			black = 0x00,
			darkerBlue = 0x10,
			darkGreen = 0x20,
			lightBlue = 0x30,
			darkRed = 0x40,
			purple = 0x50,
			orange = 0x60,
			gray = 0x70,
			darkGray = 0x80,
			darkBlue = 0x90,
			green = 0xa0,
			lighterBlue = 0xb0,
			red = 0xc0,
			pink = 0xd0,
			yellow = 0xe0,
			white = 0xf0
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
			setColor(TextColor::gray, BackgroundColor::black);
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
		Console() = default;
		Console(Console const&) = delete;
		Console(Console&&) noexcept = delete;

		Console& operator=(Console const&) = delete;
		Console& operator=(Console&&) noexcept = delete;

#ifdef _CONSOLE
		TextColor m_TextColor{ TextColor::gray };
		BackgroundColor m_BackgroundColor{ BackgroundColor::black };
#endif
	};
}

#endif