#include "froch.hpp"

#include "Audio/Audio.hpp"
#include "Core.hpp"
#include "Logger/Logger.hpp"
#include "MusicImpl.hpp"

#include <SDL_mixer.h>

namespace fro
{
	Music::Implementation::Implementation(std::string_view const filePath)
		: mSDLMusic{ Mix_LoadMUS(filePath.data()), Mix_FreeMusic }
	{
		if (not mSDLMusic.get())
			exception("failed to load {} as Mix_Music ({})",
				filePath, Mix_GetError());
	}

	Mix_Music* Music::Implementation::getSDLMusic() const
	{
		return mSDLMusic.get();
	}
}