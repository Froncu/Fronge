#if not defined fro_AUDIO_SDL_H
#define fro_AUDIO_SDL_H

#include "AudioService.h"

#include <memory>

namespace fro
{
	class AudioSDL final : public AudioService
	{
	public:
		AudioSDL();

		virtual ~AudioSDL() override;

		virtual void playMusic(std::string fileName, float const volume) override;
		virtual void playEffect(std::string fileName, float const volume) override;
		virtual void pauseMusic() override;
		virtual void pauseEffect(std::string fileName) override;
		virtual void pauseAllEffects() override;
		virtual void stopMusic() override;
		virtual void stopEffect(std::string) override;
		virtual void stopAllEffects() override;

	private:
		AudioSDL(AudioSDL const&) = delete;
		AudioSDL(AudioSDL&&) noexcept = delete;

		AudioSDL& operator=(AudioSDL const&) = delete;
		AudioSDL& operator=(AudioSDL&&) noexcept = delete;

		class AudioSDLImplementation;
		std::unique_ptr<AudioSDLImplementation> m_pAudioSDLImplementation;
	};
}

#endif