#pragma once

#include "AudioService.h"

#include <memory>

namespace fro
{
	class AudioSDL final : public AudioService
	{
	public:
		AudioSDL();

		virtual ~AudioSDL() override;

		virtual void update() override;
		virtual void playMusic(std::string const& fileName, float const volume = 1.0f) override;
		virtual void playEffect(std::string const& fileName, float const volume = 1.0f) override;
		virtual void pauseMusic() override;
		virtual void pauseEffect(std::string const& fileName) override;
		virtual void pauseAllEffects() override;

	private:
		AudioSDL(AudioSDL const&) = delete;
		AudioSDL(AudioSDL&&) noexcept = delete;

		AudioSDL& operator=(AudioSDL const&) = delete;
		AudioSDL& operator=(AudioSDL&&) noexcept = delete;

		class AudioSDLImplementation;
		std::unique_ptr<AudioSDLImplementation> m_pAudioSDLImplementation;
	};
}