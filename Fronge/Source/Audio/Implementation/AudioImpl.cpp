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
					std::string const eventLogString{ event.getLogString() };

					sLoadedMusic = std::make_unique<Music>(std::move(event.descriptor));

					if (Mix_PlayMusic(sLoadedMusic->getImplementation().getSDLMusic(), -1) == 0)
					{
						sMusic = sLoadedMusic.get();
						Logger::info("{} processed!", eventLogString);
					}
					else
					{
						sLoadedMusic.reset();
						Logger::warn("{} processing failed ({})", eventLogString, Mix_GetError());
					}
				},

				[](PlayMusicEvent const& event)
				{
					if (not event.music.valid())
					{
						Logger::warn("{} processing failed (passed Reference to Music is not valid)",
							event.getLogString());

						return;
					}

					if (Mix_PlayMusic(event.music->getImplementation().getSDLMusic(), -1) == 0)
					{
						sMusic = event.music;
						sLoadedMusic.reset();
						Logger::info("{} processed!", event.getLogString());
					}
					else
						Logger::warn("{} processing failed ({})", event.getLogString(), Mix_GetError());
				},

				[](PauseMusicEvent const& event)
				{
					Mix_PauseMusic();
					Logger::info("{} processed!", event.getLogString());
				},

				[](ResumeMusicEvent const& event)
				{
					Mix_ResumeMusic();
					Logger::info("{} processed!", event.getLogString());
				},

				[](StopMusicEvent const& event)
				{
					Mix_HaltMusic();
					sLoadedMusic.reset();
					Logger::info("{} processed!", event.getLogString());
				},

				[](LoadPlaySoundEffectEvent& event)
				{
					std::string const eventLogString{ event.getLogString() };

					int channel{ event.channel };
					auto loadedSoundEffect{ std::make_unique<SoundEffect>(std::move(event.descriptor)) };

					channel = Mix_PlayChannel(channel, loadedSoundEffect->getImplementation().getSDLSoundEffect(), 1);
					if (channel not_eq -1)
					{
						sSoundEffects[channel] = loadedSoundEffect.get();
						sLoadedSoundEffects[channel] = std::move(loadedSoundEffect);
						Logger::info("{} processed!", eventLogString);
					}
					else
					{
						loadedSoundEffect.reset();
						Logger::warn("{} processing failed ({})", eventLogString, Mix_GetError());
					}
				},

				[](PlaySoundEffectEvent const& event)
				{
					Reference<SoundEffect> const& soundEffect{ event.soundEffect };

					if (not soundEffect.valid())
					{
						Logger::warn("{} processing failed (passed Reference to SoundEffect is not valid)",
							event.getLogString());

						return;
					}

					int& channel{ soundEffect->mChannel };
					channel = Mix_PlayChannel(event.channel, soundEffect->getImplementation().getSDLSoundEffect(), 1);
					if (channel not_eq -1)
					{
						sSoundEffects[channel] = soundEffect;
						sLoadedSoundEffects[channel].reset();
						Logger::info("{} processed!", event.getLogString());
					}
					else
						Logger::warn("{} processing failed ({})", event.getLogString(), Mix_GetError());
				},

				[](PauseSoundEffectEvent const& event)
				{
					Mix_Pause(event.channel);
					Logger::info("{} processed!", event.getLogString());
				},

				[](ResumeSoundEffectEvent const& event)
				{
					Mix_Resume(event.channel);
					Logger::info("{} processed!", event.getLogString());
				},

				[](StopSoundEffectEvent const& event)
				{
					int const channel{ event.channel };
					Mix_HaltChannel(channel);

					sLoadedSoundEffects[channel].reset();
					Logger::info("{} processed!", event.getLogString());
				}
			}, event);
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

	void Audio::playMusic(Music::Descriptor music)
	{
		Implementation::pushEvent<LoadPlayMusicEvent>(std::move(music));
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

	void Audio::playSoundEffect(SoundEffect::Descriptor soundEffect, int const channel)
	{
		FRO_ASSERT(channel >= -1 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		Implementation::pushEvent<LoadPlaySoundEffectEvent>(std::move(soundEffect), channel);
	}

	void Audio::playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel)
	{
		FRO_ASSERT(channel >= -1 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		Implementation::pushEvent<PlaySoundEffectEvent>(soundEffect, channel);
	}

	void Audio::pauseSoundEffect(int const channel)
	{
		FRO_ASSERT(channel >= -1 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		Implementation::pushEvent<PauseSoundEffectEvent>(channel);
	}

	void Audio::resumeSoundEffect(int const channel)
	{
		FRO_ASSERT(channel >= -1 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

		Implementation::pushEvent<ResumeSoundEffectEvent>(channel);
	}

	void Audio::stopSoundEffect(int const channel)
	{
		FRO_ASSERT(channel >= -1 and channel < getAmountOfChannels(),
			"channel {} is out of range!", channel);

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