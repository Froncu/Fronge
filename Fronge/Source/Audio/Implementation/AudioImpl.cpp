#include "froch.hpp"

#include "AudioImpl.hpp"
#include "Core.hpp"
#include "Logger/Logger.hpp"
#include "Resources/Implementations/MusicImpl.hpp"
#include "Resources/Implementations/SoundEffectImpl.hpp"

#include <SDL_mixer.h>

namespace fro
{
	EventQueue<AudioEvent, decltype(&Audio::Implementation::processEvent)>
		Audio::Implementation::sEventQueue{ processEvent };

	std::mutex Audio::Implementation::sMutex{};
	std::condition_variable Audio::Implementation::sConditionVariable{};
	std::jthread Audio::Implementation::sEventProcessingThread{ startPollingEvents };

	bool Audio::Implementation::sRunThread{ true };

	void Audio::Implementation::stopThread()
	{
		sRunThread = false;
		sConditionVariable.notify_one();
	}

	void Audio::Implementation::startPollingEvents()
	{
		while (true)
		{
			std::unique_lock uniqueLock{ sMutex };
			sConditionVariable.wait(uniqueLock,
				[]()
				{
					return not sEventQueue.getQueue().empty() or not sRunThread;
				});

			if (not sRunThread)
				break;

			sEventQueue.fetchNextEvent();
			uniqueLock.unlock();
			sEventQueue.processEvent();
		}
	}

	void Audio::Implementation::processEvent(AudioEvent&& event)
	{
		std::visit(VariantVisitor
			{
				[](LoadPlayMusicEvent& event)
				{
					auto loadedMusic{ std::make_unique<Music>(std::move(event.filePath)) };

					if (Mix_PlayMusic(loadedMusic->getImplementation().getSDLMusic(), -1) == 0)
					{
						Logger::info("loaded and playing music with ID {}!",
							loadedMusic->getID());

						sMusic = loadedMusic.get();
						sLoadedMusic = std::move(loadedMusic);
					}
					else
						Logger::warn("failed to play music with ID {} ({})",
							loadedMusic->getID(), Mix_GetError());
				},

				[](PlayMusicEvent const& event)
				{
					Reference<Music> const& music{ event.music };

					if (not music.valid())
						Logger::warn("failed to play music (passed Reference to Music is not valid)");
					else if (Mix_PlayMusic(music->getImplementation().getSDLMusic(), -1) == 0)
					{
						Logger::info("playing music with ID {}!",
							music->getID());

						sMusic = music;
						sLoadedMusic.reset();
					}
					else
						Logger::warn("failed to play music with ID {} ({})",
							music->getID(), Mix_GetError());
				},

				[](PauseMusicEvent const&)
				{
					if (not sMusic.valid())
						Logger::warn("failed to pause music (no active music)");
					else if (Mix_PausedMusic())
						Logger::warn("failed to pause music (music is not playing)");
					else
						Logger::info("pausing music with ID {}!",
							sMusic->getID());

					Mix_PauseMusic();
				},

				[](ResumeMusicEvent const&)
				{
					if (not sMusic.valid())
						Logger::warn("failed to resume music (no active music)");
					else if (not Mix_PausedMusic())
						Logger::warn("failed to resume music with ID {} (music is already playing)",
							sMusic->getID());
					else
						Logger::info("resuming music with ID {}!",
							sMusic->getID());

					Mix_ResumeMusic();
				},

				[](StopMusicEvent const&)
				{
					if (not sMusic.valid())
						Logger::warn("failed to stop music (no active music)");
					else
						Logger::info("stopping music with ID {}!",
							sMusic->getID());

					Mix_HaltMusic();
					sMusic.reset();
					sLoadedMusic.reset();
				},

				[](LoadPlaySoundEffectEvent& event)
				{
					auto loadedSoundEffect{ std::make_unique<SoundEffect>(std::move(event.filePath)) };

					int const channel{ Mix_PlayChannel(event.channel, loadedSoundEffect->getImplementation().getSDLSoundEffect(), 1) };
					if (channel not_eq -1)
					{
						Logger::info("loaded and playing sound effect with ID {} on channel {}!",
							loadedSoundEffect->getID(), channel);

						sSoundEffects[channel] = loadedSoundEffect.get();
						sLoadedSoundEffects[channel] = std::move(loadedSoundEffect);
					}
					else
					{
						if (event.channel == -1)
							Logger::warn("failed to play sound effect with ID {} on first free channel ({})",
								loadedSoundEffect->getID(), Mix_GetError());
						else
							Logger::warn("failed to play sound effect with ID {} on channel {} ({})",
								loadedSoundEffect->getID(), event.channel, Mix_GetError());

						loadedSoundEffect.reset();
					}
				},

				[](PlaySoundEffectEvent const& event)
				{
					Reference<SoundEffect> const& soundEffect{ event.soundEffect };

					if (not soundEffect.valid())
						Logger::warn("failed to play sound effect (passed Reference to SoundEffect is not valid)");
					else
					{
						int& channel{ soundEffect->mChannel };
						channel = Mix_PlayChannel(event.channel, soundEffect->getImplementation().getSDLSoundEffect(), 1);
						if (channel not_eq -1)
						{
							Logger::info("playing sound effect with ID {} on channel {}!",
								soundEffect->getID(), channel);

							sSoundEffects[channel] = soundEffect;
							sLoadedSoundEffects[channel].reset();
						}
						else if (event.channel == -1)
							Logger::warn("failed to play sound effect with ID {} on first free channel ({})",
								soundEffect->getID(), Mix_GetError());
						else
							Logger::warn("failed to play sound effect with ID {} on channel {} ({})",
								soundEffect->getID(), event.channel, Mix_GetError());
					}
				},

				[](PauseSoundEffectEvent const& event)
				{
					if (event.channel not_eq -1)
						pauseSoundEffect(event.channel);
					else
					{
						Logger::info("pausing all sound effect channels...");
						for (int channel{}; channel < getAmountOfChannels(); ++channel)
							pauseSoundEffect(channel);
					}
				},

				[](ResumeSoundEffectEvent const& event)
				{
					if (event.channel not_eq -1)
						resumeSoundEffect(event.channel);
					else
					{
						Logger::info("resuming all sound effect channels...");
						for (int channel{}; channel < getAmountOfChannels(); ++channel)
							resumeSoundEffect(channel);
					}
				},

				[](StopSoundEffectEvent const& event)
				{
					if (event.channel not_eq -1)
						stopSoundEffect(event.channel);
					else
					{
						Logger::info("stopping all sound effect channels...");
						for (int channel{}; channel < getAmountOfChannels(); ++channel)
							stopSoundEffect(channel);
					}
				}
			}, event);
	}

	void Audio::Implementation::pauseSoundEffect(int const channel)
	{
		Reference<SoundEffect> const& soundEffect{ sSoundEffects[channel] };

		if (not soundEffect.valid())
			Logger::warn("failed to pause sound effect on channel {} (no active sound effect in channel)",
				channel);
		else if (Mix_Paused(channel))
			Logger::warn("failed to pause sound effect with ID {} on channel {} (channel is not playing)",
				soundEffect->getID(), channel);
		else
			Logger::info("pausing sound effect with ID {} on channel {}!",
				soundEffect->getID(), channel);

		Mix_Pause(channel);
	}

	void Audio::Implementation::resumeSoundEffect(int const channel)
	{
		Reference<SoundEffect> const& soundEffect{ sSoundEffects[channel] };

		if (not soundEffect.valid())
			Logger::warn("failed to resume sound effect on channel {} (no active sound effect in channel)",
				channel);
		else if (not Mix_Paused(channel))
			Logger::warn("failed to resume sound effect with ID {} on channel {} (channel is already playing)",
				soundEffect->getID(), channel);
		else
			Logger::info("pausing sound effect with ID {} on channel {}!",
				soundEffect->getID(), channel);

		Mix_Resume(channel);
	}

	void Audio::Implementation::stopSoundEffect(int const channel)
	{
		Reference<SoundEffect> const& soundEffect{ sSoundEffects[channel] };

		if (not soundEffect.valid())
			Logger::warn("failed to stop sound effect on channel {} (no active sound effect)",
				channel);
		else
			Logger::info("stopping sound effect with ID {} on channel {}!",
				soundEffect->getID(), channel);

		Mix_HaltChannel(channel);
		sLoadedSoundEffects[channel].reset();
	}

	Reference<Music> Audio::sMusic{};
	std::vector<Reference<SoundEffect>> Audio::sSoundEffects(MIX_CHANNELS);

	std::unique_ptr<Music> Audio::sLoadedMusic{};
	std::vector<std::unique_ptr<SoundEffect>> Audio::sLoadedSoundEffects(MIX_CHANNELS);

	void Audio::initialize()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
			FRO_EXCEPTION("failed to open audio device ({})", Mix_GetError());

		fro::Logger::info("Audio initialized!");
	}

	void Audio::shutDown()
	{
		Mix_CloseAudio();
		Implementation::stopThread();

		fro::Logger::info("Audio shut down!");
	}

	void Audio::playMusic(std::string filePath)
	{
		Implementation::pushEvent<LoadPlayMusicEvent>(std::move(filePath));
	}

	void Audio::playMusic(Reference<Music> const music)
	{
		Implementation::pushEvent<PlayMusicEvent>(music);;
	}

	void Audio::pauseMusic()
	{
		Implementation::pushEvent<PauseMusicEvent>();
	}

	void Audio::resumeMusic()
	{
		Implementation::pushEvent<ResumeMusicEvent>();
	}

	void Audio::stopMusic()
	{
		Implementation::pushEvent<StopMusicEvent>();
	}

	void Audio::playSoundEffect(std::string filePath, int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			FRO_EXCEPTION("channel {} is out of range!", channel);

		Implementation::pushEvent<LoadPlaySoundEffectEvent>(std::move(filePath), channel);
	}

	void Audio::playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			FRO_EXCEPTION("channel {} is out of range!", channel);

		Implementation::pushEvent<PlaySoundEffectEvent>(soundEffect, channel);
	}

	void Audio::pauseSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			FRO_EXCEPTION("channel {} is out of range!", channel);

		Implementation::pushEvent<PauseSoundEffectEvent>(channel);
	}

	void Audio::resumeSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			FRO_EXCEPTION("channel {} is out of range!", channel);

		Implementation::pushEvent<ResumeSoundEffectEvent>(channel);
	}

	void Audio::stopSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			FRO_EXCEPTION("channel {} is out of range!", channel);

		Implementation::pushEvent<StopSoundEffectEvent>(channel);
	}

	Reference<Music> Audio::getMixedMusic()
	{
		return sMusic;
	}

	Reference<SoundEffect> Audio::getSoundEffect(int const channel)
	{
		FRO_ASSERT(channel >= 0 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		return sSoundEffects[channel];
	}

	int Audio::getAmountOfChannels()
	{
		return MIX_CHANNELS;
	}
}