#include "froch.hpp"

#include "FontImpl.hpp"

#include <SDL_ttf.h>

namespace fro
{
	CustomUniquePointer<TTF_Font> Font::Implementation::createFont(std::string_view const filePath, int const size)
	{
		if (TTF_Init() == -1)
			FRO_EXCEPTION("failed to initalized SDL_ttf ({})",
				TTF_GetError());

		if (TTF_WasInit() == 1)
			Logger::info("initialized font library!");

		return { TTF_OpenFont(filePath.data(), size),
			[](TTF_Font* const font)
			{
				TTF_CloseFont(font);
				TTF_Quit();

				if (not TTF_WasInit())
					Logger::info("quit font library!");
			} };
	}

	Font::Implementation::Implementation(std::string_view const filePath, int const size)
		: mSDLFont{ createFont(filePath, size) }
	{
		if (not mSDLFont.get())
			FRO_EXCEPTION("failed to load {} as TTF_Font ({})",
				filePath, TTF_GetError());
	}

	TTF_Font* Font::Implementation::getSDLFont() const
	{
		return mSDLFont.get();
	}

	IDGenerator Font::sIDGenerator{};

	Font::Font(std::string_view const filePath, int const size)
		: mImplementation{ std::make_unique<Implementation>(filePath, size) }
	{
		Logger::info("{} loaded as font with ID {}!",
			filePath, mID);
	}

	Font::Font(Font&& other) noexcept
		: mImplementation{ std::move(other.mImplementation) }
	{
	}

	Font::~Font()
	{
		Logger::info("font with ID {} destroyed!",
			mID);
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		if (this == &other)
			return *this;

		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	Font::Implementation& Font::getImplementation() const
	{
		return *mImplementation;
	}
}