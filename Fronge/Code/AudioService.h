#pragma once

#include <string>

namespace fro
{
	class AudioService
	{
	public:
		AudioService() = default;

		virtual ~AudioService() = default;

		virtual void update();

		virtual void playMusic(const std::string& fileName, float volume = 1.0f);
		virtual void playEffect(const std::string& fileName, float volume = 1.0f);
		virtual void pauseMusic();
		virtual void pauseEffect(const std::string& fileName);
		virtual void pauseAllEffects();

	private:
		AudioService(AudioService const&) = delete;
		AudioService(AudioService&&) noexcept = delete;

		AudioService& operator=(AudioService const&) = delete;
		AudioService& operator=(AudioService&&) noexcept = delete;
	};
}