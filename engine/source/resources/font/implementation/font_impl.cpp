#include "froch.hpp"

#include "FontImpl.hpp"
#include "Utility/Exception.hpp"

#include <SDL_ttf.h>

namespace fro
{
	CustomUniquePointer<TTF_Font> Font::Implementation::createFont(std::string_view const filePath, int const size)
	{
		if (TTF_Init() == -1)
			exception("failed to initalize SDL_ttf ({})",
				TTF_GetError());

		if (TTF_WasInit() == 1)
			Logger::info("initialized SDL_ttf!");

		return { TTF_OpenFont(filePath.data(), size),
			[](TTF_Font* const font)
			{
				TTF_CloseFont(font);
				TTF_Quit();

				if (not TTF_WasInit())
					Logger::info("quit SDL_ttf!");
			} };
	}

	Font::Implementation::Implementation(std::string_view const filePath, int const size)
		: mSDLFont{ createFont(filePath, size) }
	{
		if (not mSDLFont.get())
			exception("failed to load {} as TTF_Font ({})",
				filePath, TTF_GetError());
	}

	TTF_Font* Font::Implementation::getSDLFont() const
	{
		return mSDLFont.get();
	}
}