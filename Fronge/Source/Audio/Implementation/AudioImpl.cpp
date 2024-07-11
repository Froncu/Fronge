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
						Logger::info("loaded and played Music with ID {}!",
							loadedMusic->getID());

						sActiveMusic = loadedMusic.get();
						sLoadedMusic = std::move(loadedMusic);
					}
					else
						Logger::warn("failed to play Music with ID {} ({})",
							loadedMusic->getID(), Mix_GetError());
				},

				[](PlayMusicEvent const& event)
				{
					Reference<Music> const& music{ event.music };

					if (not music.valid())
						Logger::warn("failed to play Music (passed Reference to Music is not valid)");
					else if (Mix_PlayMusic(music->getImplementation().getSDLMusic(), -1) == 0)
					{
						Logger::info("played Music with ID {}!",
							music->getID());

						sActiveMusic = music;
						sLoadedMusic.reset();
					}
					else
						Logger::warn("failed to play Music with ID {} ({})",
							music->getID(), Mix_GetError());
				},

				[](PauseMusicEvent const&)
				{
					if (not sActiveMusic.valid())
						Logger::warn("failed to pause Music (no active Music)");
					else if (Mix_PausedMusic())
						Logger::warn("failed to pause Music (Music is not playing)");
					else
						Logger::info("paused music with ID {}!",
							sActiveMusic->getID());

					Mix_PauseMusic();
				},

				[](ResumeMusicEvent const&)
				{
					if (not sActiveMusic.valid())
						Logger::warn("failed to resume Music (no active Music)");
					else if (not Mix_PausedMusic())
						Logger::warn("failed to resume Music with ID {} (music is already playing)",
							sActiveMusic->getID());
					else
						Logger::info("resumed Music with ID {}!",
							sActiveMusic->getID());

					Mix_ResumeMusic();
				},

				[](StopMusicEvent const&)
				{
					if (not sActiveMusic.valid())
						Logger::warn("failed to stop music (no active Music)");
					else
						Logger::info("stopped Music with ID {}!",
							sActiveMusic->getID());

					Mix_HaltMusic();
					sActiveMusic.reset();
					sLoadedMusic.reset();
				},

				[](LoadPlaySoundEffectEvent& event)
				{
					auto loadedSoundEffect{ std::make_unique<SoundEffect>(std::move(event.filePath)) };

					int const channel{ Mix_PlayChannel(event.channel, loadedSoundEffect->getImplementation().getSDLSoundEffect(), 1) };
					if (channel not_eq -1)
					{
						Logger::info("loaded and played SoundEffect with ID {} on channel {}!",
							loadedSoundEffect->getID(), channel);

						sActiveSoundEffects[channel] = loadedSoundEffect.get();
						sLoadedSoundEffects[channel] = std::move(loadedSoundEffect);
					}
					else
					{
						if (event.channel == -1)
							Logger::warn("failed to play SoundEffect with ID {} on first free channel ({})",
								loadedSoundEffect->getID(), Mix_GetError());
						else
							Logger::warn("failed to play SoundEffect with ID {} on channel {} ({})",
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
							Logger::info("played SoundEffect with ID {} on channel {}!",
								soundEffect->getID(), channel);

							sActiveSoundEffects[channel] = soundEffect;
							sLoadedSoundEffects[channel].reset();
						}
						else if (event.channel == -1)
							Logger::warn("failed to play SoundEffect with ID {} on first free channel ({})",
								soundEffect->getID(), Mix_GetError());
						else
							Logger::warn("failed to play SoundEffect with ID {} on channel {} ({})",
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
		Reference<SoundEffect> const& activeSoundEffect{ sActiveSoundEffects[channel] };

		if (not activeSoundEffect.valid())
			Logger::warn("failed to pause sound effect on channel {} (no active SoundEffect in channel)",
				channel);
		else if (Mix_Paused(channel))
			Logger::warn("failed to pause SoundEffect with ID {} on channel {} (channel is not playing)",
				activeSoundEffect->getID(), channel);
		else
			Logger::info("paused SoundEffect with ID {} on channel {}!",
				activeSoundEffect->getID(), channel);

		Mix_Pause(channel);
	}

	void Audio::Implementation::resumeSoundEffect(int const channel)
	{
		Reference<SoundEffect> const& activeSoundEffect{ sActiveSoundEffects[channel] };

		if (not activeSoundEffect.valid())
			Logger::warn("failed to resume sound effect on channel {} (no active SoundEffect in channel)",
				channel);
		else if (not Mix_Paused(channel))
			Logger::warn("failed to resume SoundEffect with ID {} on channel {} (channel is already playing)",
				activeSoundEffect->getID(), channel);
		else
			Logger::info("paused SoundEffect with ID {} on channel {}!",
				activeSoundEffect->getID(), channel);

		Mix_Resume(channel);
	}

	void Audio::Implementation::stopSoundEffect(int const channel)
	{
		Reference<SoundEffect> const& activeSoundEffect{ sActiveSoundEffects[channel] };

		if (not activeSoundEffect.valid())
			Logger::warn("failed to stop sound effect on channel {} (no active sound effect)",
				channel);
		else
			Logger::info("stopped SoundEffect with ID {} on channel {}!",
				activeSoundEffect->getID(), channel);

		Mix_HaltChannel(channel);
		sLoadedSoundEffects[channel].reset();
	}

	std::vector<Reference<SoundEffect>> Audio::sActiveSoundEffects(MIX_CHANNELS);
	std::vector<std::unique_ptr<SoundEffect>> Audio::sLoadedSoundEffects(MIX_CHANNELS);
	Reference<Music> Audio::sActiveMusic{};
	std::unique_ptr<Music> Audio::sLoadedMusic{};

	void Audio::initialize()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
			FRO_EXCEPTION("failed to open audio device ({})", Mix_GetError());

		fro::Logger::info("initialized Audio!");
	}

	void Audio::shutDown()
	{
		Mix_CloseAudio();
		Implementation::stopThread();

		fro::Logger::info("shut down Audio!");
	}

	void Audio::playMusic(std::string filePath)
	{
		Implementation::pushEvent<LoadPlayMusicEvent>(std::move(filePath));
	}

	void Audio::playMusic(Reference<Music> const music)
	{
		Implementation::pushEvent<PlayMusicEvent>(music);
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

	Reference<Music> Audio::getActiveMusic()
	{
		return sActiveMusic;
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

	Reference<SoundEffect> Audio::getActiveSoundEffect(int const channel)
	{
		FRO_ASSERT(channel >= 0 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		return sActiveSoundEffects[channel];
	}

	int Audio::getAmountOfChannels()
	{
		return MIX_CHANNELS;
	}
}