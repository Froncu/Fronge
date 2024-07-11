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

		FRO_API static void playMusic(std::string filePath);
		FRO_API static void playMusic(Reference<Music> const music);
		FRO_API static void pauseMusic();
		FRO_API static void resumeMusic();
		FRO_API static void stopMusic();
		FRO_API FRO_NODISCARD static Reference<Music> getActiveMusic();

		FRO_API static void playSoundEffect(std::string filePath, int const channel = -1);
		FRO_API static void playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel = -1);
		FRO_API static void pauseSoundEffect(int const channel = -1);
		FRO_API static void resumeSoundEffect(int const channel = -1);
		FRO_API static void stopSoundEffect(int const channel = -1);
		FRO_API FRO_NODISCARD static Reference<SoundEffect> getActiveSoundEffect(int const channel);
		FRO_API FRO_NODISCARD static int getAmountOfChannels();

	private:
		static std::vector<Reference<SoundEffect>> sActiveSoundEffects;
		static std::vector<std::unique_ptr<SoundEffect>> sLoadedSoundEffects;
		static Reference<Music> sActiveMusic;
		static std::unique_ptr<Music> sLoadedMusic;

		Audio() = delete;
		Audio(Audio const&) = delete;
		Audio(Audio&&) noexcept = delete;

		~Audio() = delete;

		Audio& operator=(Audio const&) = delete;
		Audio& operator=(Audio&&) noexcept = delete;
	};
}

#endif