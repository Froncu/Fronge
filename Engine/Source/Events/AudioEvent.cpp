#include "froch.hpp"

#include "AudioEvent.hpp"

namespace fro
{
	std::string LoadPlayMusicEvent::getLogString() const
	{
		return std::format("load {} as Music and play event",
			filePath);
	}

	std::string PlayMusicEvent::getLogString() const
	{
		return std::format("play Music with ID {} event",
			music->getID());
	}

	std::string PauseMusicEvent::getLogString() const
	{
		return "pause Music event";
	}

	std::string ResumeMusicEvent::getLogString() const
	{
		return "resume Music event";
	}

	std::string StopMusicEvent::getLogString() const
	{
		return "stop Music event";
	}

	std::string LoadPlaySoundEffectEvent::getLogString() const
	{
		if (channel == -1)
			return std::format("load {} as SoundEffect and play on first free channel event",
				filePath);
		else
			return std::format("load {} as SoundEffect and play on channel {} event",
				filePath, channel);
	}

	std::string PlaySoundEffectEvent::getLogString() const
	{
		if (channel == -1)
			return std::format("play SoundEffect with ID {} on first free channel event",
				soundEffect->getID());
		else
			return std::format("play SoundEffect with ID {} on channel {} event",
				soundEffect->getID(), channel);
	}

	std::string PauseSoundEffectEvent::getLogString() const
	{
		return std::format("pause SoundEffect on channel {} event",
			channel);
	}

	std::string ResumeSoundEffectEvent::getLogString() const
	{
		return std::format("resume SoundEffect on channel {} event",
			channel);
	}

	std::string StopSoundEffectEvent::getLogString() const
	{
		return std::format("resume SoundEffect on channel {} event",
			channel);
	}
}