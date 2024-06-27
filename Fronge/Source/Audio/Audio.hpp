#if not defined AUDIO_HPP
#define AUDIO_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Resources/Music.hpp"
#include "Resources/SoundEffect.hpp"
#include "Reference/Reference.hpp"

namespace fro
{
	class Audio final
	{
		class Implementation;

	public:
		FRO_API static void initialize();
		FRO_API static void shutDown();

		FRO_API static void playMusic(Music::Descriptor music);
		FRO_API static void playMusic(Reference<Music> const music);
		FRO_API static void pauseMusic();
		FRO_API static void resumeMusic();
		FRO_API static void stopMusic();

		FRO_API static void playSoundEffect(SoundEffect::Descriptor soundEffect, int const channel = -1);
		FRO_API static void playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel = -1);
		FRO_API static void pauseSoundEffect(int const channel = -1);
		FRO_API static void resumeSoundEffect(int const channel = -1);
		FRO_API static void stopSoundEffect(int const channel = -1);

		FRO_API FRO_NODISCARD static Reference<Music> getMixedMusic();
		FRO_API FRO_NODISCARD static Reference<SoundEffect> getSoundEffect(int const channel);
		FRO_API FRO_NODISCARD static int getAmountOfChannels();

	private:
		static Reference<Music> sMusic;
		static std::vector<Reference<SoundEffect>> sSoundEffects;

		static std::unique_ptr<Music> sLoadedMusic;
		static std::vector<std::unique_ptr<SoundEffect>> sLoadedSoundEffects;

		Audio() = delete;
		Audio(Audio const&) = delete;
		Audio(Audio&&) noexcept = delete;

		~Audio() = delete;

		Audio& operator=(Audio const&) = delete;
		Audio& operator=(Audio&&) noexcept = delete;
	};
}

#endif