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
		FRO_API static void stopThread();

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

	private:
		static void startPollingEvents();
		static void processEvent(AudioEvent&& event);
		static void pauseSoundEffect(int const channel);
		static void resumeSoundEffect(int const channel);
		static void stopSoundEffect(int const channel);

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