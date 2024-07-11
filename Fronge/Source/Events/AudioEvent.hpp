#if not defined AUDIO_EVENT_HPP
#define AUDIO_EVENT_HPP

#include "froch.hpp"

#include "Events/Systems/Event.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Music.hpp"
#include "Resources/SoundEffect.hpp"

namespace fro
{
	struct LoadPlayMusicEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::string filePath;
	};

	struct PlayMusicEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		Reference<Music> music;
	};

	struct PauseMusicEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;
	};

	struct ResumeMusicEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;
	};

	struct StopMusicEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;
	};

	struct LoadPlaySoundEffectEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		std::string filePath;
		int channel;
	};

	struct PlaySoundEffectEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		Reference<SoundEffect> soundEffect;
		int channel;
	};

	struct PauseSoundEffectEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		int channel;
	};

	struct ResumeSoundEffectEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		int channel;
	};

	struct StopSoundEffectEvent final
	{
		FRO_API FRO_NODISCARD std::string getLogString() const;

		int channel;
	};

	using AudioEvent = EventVariant<
		LoadPlayMusicEvent,
		PlayMusicEvent,
		PauseMusicEvent,
		ResumeMusicEvent,
		StopMusicEvent,
		LoadPlaySoundEffectEvent,
		PlaySoundEffectEvent,
		PauseSoundEffectEvent,
		ResumeSoundEffectEvent,
		StopSoundEffectEvent>;
}

#endif