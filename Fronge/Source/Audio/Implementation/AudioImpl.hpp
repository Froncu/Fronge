#if not defined AUDIO_IMPL_H
#define AUDIO_IMPL_H

#include "froch.hpp"

#include "Audio/Audio.hpp"
#include "Events/AudioEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utility.hpp"

namespace fro
{
	class Audio::Implementation final
	{
	public:
		static void initialize();
		static void shutDown();

		static void playMusic(Music::Descriptor music);
		static void playMusic(Reference<Music> const music);
		static void pauseMusic();
		static void resumeMusic();
		static void stopMusic();

		static void playSoundEffect(SoundEffect::Descriptor soundEffect, int const channel);
		static void playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel);
		static void pauseSoundEffect(int const channel);
		static void resumeSoundEffect(int const channel);
		static void stopSoundEffect(int const channel);

	private:
		static void startPollingEvents();
		static void processEvent(AudioEvent&& event);

		template<typename EventType, typename... Arguments>
			requires std::constructible_from<AudioEvent, EventType> and std::constructible_from<EventType, Arguments...>
		static void pushEvent(Arguments&&... arguments)
		{
			{
				std::lock_guard const lockGuard{ sMutex };
				sEventQueue.pushEvent(EventType(std::forward<Arguments>(arguments)...));
			}

			sConditionVariable.notify_one();
		}

		static EventQueue<AudioEvent, decltype(&processEvent)> sEventQueue;

		static std::mutex sMutex;
		static std::condition_variable sConditionVariable;
		static std::jthread sEventProcessingThread;

		static bool sRunThread;

		Implementation() = delete;
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		~Implementation() = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;
	};
}

#endif