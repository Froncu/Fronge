#if not defined fro_AUDIO_SERVICE_H
#define fro_AUDIO_SERVICE_H

#include <xstring>

namespace fro
{
	class AudioService
	{
	public:
		AudioService() = default;

		virtual ~AudioService() = default;

		virtual void playMusic(std::string_view const fileName, float const volume = 1.0f);
		virtual void playEffect(std::string_view const fileName, float const volume = 1.0f);
		virtual void pauseMusic();
		virtual void pauseEffect(std::string_view const fileName);
		virtual void pauseAllEffects();

	private:
		AudioService(AudioService const&) = delete;
		AudioService(AudioService&&) noexcept = delete;

		AudioService& operator=(AudioService const&) = delete;
		AudioService& operator=(AudioService&&) noexcept = delete;
	};
}

#endif