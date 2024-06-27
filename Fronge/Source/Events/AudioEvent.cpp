#include "froch.hpp"

#include "AudioEvent.hpp"

namespace fro
{
	std::string LoadPlayMusicEvent::getLogString() const
	{
		return std::format("load {} as music and play event",
			descriptor.filePath);
	}

	std::string PlayMusicEvent::getLogString() const
	{
		return std::format("play {} music event",
			music->getFilePath());
	}

	std::string PauseMusicEvent::getLogString() const
	{
		return "pause music event";
	}

	std::string ResumeMusicEvent::getLogString() const
	{
		return "resume music event";
	}

	std::string StopMusicEvent::getLogString() const
	{
		return "stop music event";
	}

	std::string LoadPlaySoundEffectEvent::getLogString() const
	{
		return std::format("load {} as sound effect and play on channel {} event",
			descriptor.filePath, channel);
	}

	std::string PlaySoundEffectEvent::getLogString() const
	{
		return std::format("play {} sound effect on channel {} event",
			soundEffect->getFilePath(), soundEffect->getChannel());
	}

	std::string PauseSoundEffectEvent::getLogString() const
	{
		return std::format("pause sound effect on channel {} event",
			channel);
	}

	std::string ResumeSoundEffectEvent::getLogString() const
	{
		return std::format("resume sound effect on channel {} event",
			channel);
	}

	std::string StopSoundEffectEvent::getLogString() const
	{
		return std::format("resume sound effect on channel {} event",
			channel);
	}
}