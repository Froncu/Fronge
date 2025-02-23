#include "froch.hpp"

#include "Core.hpp"
#include "SoundEffectImpl.hpp"
#include "Utility/Exception.hpp"

#include <SDL_mixer.h>

namespace fro
{
	SoundEffect::Implementation::Implementation(std::string_view const filePath)
		: mSDLSoundEffect{ Mix_LoadWAV(filePath.data()), Mix_FreeChunk }
	{
		if (not mSDLSoundEffect.get())
			exception("failed to load {} as Mix_Chunk ({})",
				filePath, Mix_GetError());
	}

	Mix_Chunk* SoundEffect::Implementation::getSDLSoundEffect() const
	{
		return mSDLSoundEffect.get();
	}
}