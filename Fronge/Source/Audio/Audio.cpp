#include "froch.hpp"

#include "Audio.hpp"
#include "Resources/Music/Implementation/MusicImpl.hpp"
#include "Resources/SoundEffect/Implementation/SoundEffectImpl.hpp"
#include "Utility/Assert.hpp"
#include "Utility/VariantVisitor.hpp"

#include <SDL_mixer.h>

namespace fro
{
	void Audio::initialize()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
			exception("failed to open audio device ({})", Mix_GetError());

		Logger::info("initialized Audio!");
	}

	void Audio::shutDown()
	{
		Mix_CloseAudio();

		sRunThread = false;
		sConditionVariable.notify_one();

		Logger::info("shut down Audio!");
	}

	void Audio::playMusic(std::string filePath)
	{
		pushEvent<LoadPlayMusicEvent>(std::move(filePath));
	}

	void Audio::playMusic(Reference<Music> const music)
	{
		pushEvent<PlayMusicEvent>(music);
	}

	void Audio::pauseMusic()
	{
		pushEvent<PauseMusicEvent>();
	}

	void Audio::resumeMusic()
	{
		pushEvent<ResumeMusicEvent>();
	}

	void Audio::stopMusic()
	{
		pushEvent<StopMusicEvent>();
	}

	Reference<Music> Audio::getActiveMusic()
	{
		return sActiveMusic;
	}

	void Audio::playSoundEffect(std::string filePath, int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			exception("channel {} is out of range!", channel);

		pushEvent<LoadPlaySoundEffectEvent>(std::move(filePath), channel);
	}

	void Audio::playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			exception("channel {} is out of range!", channel);

		pushEvent<PlaySoundEffectEvent>(soundEffect, channel);
	}

	void Audio::pauseSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			exception("channel {} is out of range!", channel);

		pushEvent<PauseSoundEffectEvent>(channel);
	}

	void Audio::resumeSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			exception("channel {} is out of range!", channel);

		pushEvent<ResumeSoundEffectEvent>(channel);
	}

	void Audio::stopSoundEffect(int const channel)
	{
		if (channel < -1 or channel >= getAmountOfChannels())
			exception("channel {} is out of range!", channel);

		pushEvent<StopSoundEffectEvent>(channel);
	}

	Reference<SoundEffect> Audio::getActiveSoundEffect(int const channel)
	{
		assert(channel >= 0 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		return sActiveSoundEffects[channel];
	}

	int Audio::getAmountOfChannels()
	{
		return MIX_CHANNELS;
	}

	void Audio::setMute(bool const mute)
	{
		sMuted = mute;
		Mix_MasterVolume(sMuted ? 0 : MIX_MAX_VOLUME);
		Mix_VolumeMusic(sMuted ? 0 : MIX_MAX_VOLUME);
	}

	bool Audio::isMuted()
	{
		return sMuted;
	}

	void Audio::internalPauseSoundEffect(int const channel)
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

	void Audio::internalResumeSoundEffect(int const channel)
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

	void Audio::internalStopSoundEffect(int const channel)
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

	EventQueue<AudioEvent> Audio::sEventQueue
	{
		VariantVisitor
		{
			[](LoadPlayMusicEvent&& event)
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

			[](PlayMusicEvent&& event)
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

			[](PauseMusicEvent&&)
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

			[](ResumeMusicEvent&&)
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

			[](StopMusicEvent&&)
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

			[](LoadPlaySoundEffectEvent&& event)
			{
				auto loadedSoundEffect{ std::make_unique<SoundEffect>(std::move(event.filePath)) };

				int const channel{ Mix_PlayChannel(event.channel, loadedSoundEffect->getImplementation().getSDLSoundEffect(), 0) };
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

			[](PlaySoundEffectEvent&& event)
			{
				Reference<SoundEffect> const& soundEffect{ event.soundEffect };

				if (not soundEffect.valid())
					Logger::warn("failed to play sound effect (passed Reference to SoundEffect is not valid)");
				else
				{
					int& channel{ soundEffect->mChannel };
					channel = Mix_PlayChannel(event.channel, soundEffect->getImplementation().getSDLSoundEffect(), 0);
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

			[](PauseSoundEffectEvent&& event)
			{
				if (event.channel not_eq -1)
					internalPauseSoundEffect(event.channel);
				else
				{
					Logger::info("pausing all sound effect channels...");
					for (int channel{}; channel < getAmountOfChannels(); ++channel)
						internalPauseSoundEffect(channel);
				}
			},

			[](ResumeSoundEffectEvent&& event)
			{
				if (event.channel not_eq -1)
					internalResumeSoundEffect(event.channel);
				else
				{
					Logger::info("resuming all sound effect channels...");
					for (int channel{}; channel < getAmountOfChannels(); ++channel)
						internalResumeSoundEffect(channel);
				}
			},

			[](StopSoundEffectEvent&& event)
			{
				if (event.channel not_eq -1)
					internalStopSoundEffect(event.channel);
				else
				{
					Logger::info("stopping all sound effect channels...");
					for (int channel{}; channel < getAmountOfChannels(); ++channel)
						internalStopSoundEffect(channel);
				}
			}
		}
	};

	std::jthread Audio::sEventProcessingThread
	{
		[]()
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
	};

	std::mutex Audio::sMutex{};
	std::condition_variable Audio::sConditionVariable{};
	std::vector<Reference<SoundEffect>> Audio::sActiveSoundEffects(MIX_CHANNELS);
	std::vector<std::unique_ptr<SoundEffect>> Audio::sLoadedSoundEffects(MIX_CHANNELS);
	Reference<Music> Audio::sActiveMusic{};
	std::unique_ptr<Music> Audio::sLoadedMusic{};
	bool Audio::sRunThread{ true };
	bool Audio::sMuted{};
}