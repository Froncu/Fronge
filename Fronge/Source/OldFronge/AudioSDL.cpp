#include "AudioSDL.h"

#include "EventQueue.hpp"
#include "ResourceManager.h"

#include <sdl_mixer.h>

#include <condition_variable>
#include <format>
#include <stdexcept>
#include <thread>
#include <xstring>
#include <unordered_map>

#pragma region Constructors/Destructor
fro::AudioSDL::AudioSDL()
	: m_pAudioSDLImplementation{ std::make_unique<AudioSDLImplementation>() }
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) not_eq 0)
		throw std::runtime_error(std::format("Mix_OpenAudio() failed: {}", Mix_GetError()));
}

fro::AudioSDL::~AudioSDL()
{
	Mix_CloseAudio();
}
#pragma endregion Constructors/Destructor



#pragma region Implementation
class fro::AudioSDL::AudioSDLImplementation final
{
public:
	AudioSDLImplementation()
	{
		Mix_ChannelFinished(channelFreed);
	};

	~AudioSDLImplementation()
	{
		m_RunThread = false;
		m_ConditionVariable.notify_one();
	}

	void playMusic(std::string fileName, float const volume)
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{ std::move(fileName) },
					.volume{ volume },
					.isMusic{ true },
					.type{ AudioEvent::Type::play }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void playEffect(std::string fileName, float const volume)
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{ std::move(fileName) },
					.volume{ volume },
					.isMusic{ false },
					.type{ AudioEvent::Type::play }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void pauseMusic()
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{},
					.volume{},
					.isMusic{ true },
					.type{ AudioEvent::Type::pause }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void pauseEffect(std::string fileName)
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{ std::move(fileName) },
					.volume{},
					.isMusic{ false },
					.type{ AudioEvent::Type::pause }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void pauseAllEffects()
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{},
					.volume{},
					.isMusic{ false },
					.type{ AudioEvent::Type::pause }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void stopMusic()
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{},
					.volume{},
					.isMusic{ true },
					.type{ AudioEvent::Type::stop }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void stopEffect(std::string fileName)
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{ std::move(fileName) },
					.volume{},
					.isMusic{ false },
					.type{ AudioEvent::Type::stop }
				});
		}

		m_ConditionVariable.notify_one();
	}

	void stopAllEffects()
	{
		{
			std::lock_guard const lockGuard{ m_Mutex };
			m_EventQueue.pushEvent(AudioEvent
				{
					.fileName{},
					.volume{},
					.isMusic{ false },
					.type{ AudioEvent::Type::stop }
				});
		}

		m_ConditionVariable.notify_one();
	}

private:
	struct AudioEvent final
	{
	public:
		std::string fileName;
		float volume;
		bool isMusic;

		enum class Type
		{
			play,
			pause,
			stop
		} type;

		bool operator==(AudioEvent const& event) const
		{
			return
				this->fileName == event.fileName &&
				this->isMusic == event.isMusic &&
				this->type == event.type;
		}
	};

	AudioSDLImplementation(AudioSDLImplementation const&) = delete;
	AudioSDLImplementation(AudioSDLImplementation&&) noexcept = delete;

	AudioSDLImplementation& operator=(AudioSDLImplementation const&) = delete;
	AudioSDLImplementation& operator=(AudioSDLImplementation&&) noexcept = delete;

	static std::unordered_map<std::string, int> m_mEFFECT_CHANNELS;

	std::mutex m_Mutex{};
	std::condition_variable m_ConditionVariable{};
	bool m_RunThread{ true };

	EventQueue<AudioEvent> m_EventQueue
	{
		[](AudioEvent&& event)
		{
			if (event.isMusic)
			{
				static std::string CURRENT_MUSIC_NAME{};

				switch (event.type)
				{
				case AudioEvent::Type::play:
					if (CURRENT_MUSIC_NAME not_eq event.fileName)
					{
						CURRENT_MUSIC_NAME = std::move(event.fileName);

						Mix_VolumeMusic(static_cast<int>(event.volume * MIX_MAX_VOLUME));
						Mix_PlayMusic(ResourceManager::getInstance().getMusic(CURRENT_MUSIC_NAME), -1);
					}
					else
						Mix_ResumeMusic();
					break;

				case AudioEvent::Type::pause:
					Mix_PauseMusic();
					break;

				case AudioEvent::Type::stop:
					Mix_HaltMusic();
					CURRENT_MUSIC_NAME.clear();
					break;
				}
			}
			else
			{
				switch (event.type)
				{
				case AudioEvent::Type::play:
					if (auto const iFoundChannel{ m_mEFFECT_CHANNELS.find(std::move(event.fileName)) };
						iFoundChannel == m_mEFFECT_CHANNELS.end())
					{
						Mix_Chunk* const pEffect{ ResourceManager::getInstance().getEffect(event.fileName) };

						Mix_VolumeChunk(pEffect, static_cast<int>(event.volume * MIX_MAX_VOLUME));
						m_mEFFECT_CHANNELS[std::move(event.fileName)] = Mix_PlayChannel(-1, pEffect, 0);
					}
					else
						Mix_Resume(iFoundChannel->second);
					break;

				case AudioEvent::Type::pause:
				case AudioEvent::Type::stop:
					if (not event.fileName.empty())
					{
						auto const iterator{ m_mEFFECT_CHANNELS.find(event.fileName) };
						if (iterator == m_mEFFECT_CHANNELS.end())
							return;

						if (event.type == AudioEvent::Type::pause)
							Mix_Pause(iterator->second);
						else
							Mix_HaltChannel(iterator->second);
					}
					else
						if (event.type == AudioEvent::Type::pause)
							Mix_Pause(-1);
						else
							Mix_HaltChannel(-1);
					break;
				}
			}
		}
	};

	std::jthread m_EventProcessingThread
	{
		[this]()
		{
			while (true)
			{
				std::unique_lock uniqueLock{ m_Mutex };

				m_ConditionVariable.wait(uniqueLock,
					[this]()
					{
						return not m_EventQueue.getQueue().empty() or not m_RunThread;
					});

				if (not m_RunThread)
					break;

				m_EventQueue.fetchNextEvent();

				uniqueLock.unlock();

				m_EventQueue.processEvent();
			}
		}
	};

	static void channelFreed(int channel)
	{
		m_mEFFECT_CHANNELS.erase(std::find_if(m_mEFFECT_CHANNELS.begin(), m_mEFFECT_CHANNELS.end(),
			[channel](auto const& effectChannel)
			{
				return effectChannel.second == channel;
			}));
	}
};

std::unordered_map<std::string, int> fro::AudioSDL::AudioSDLImplementation::m_mEFFECT_CHANNELS{};
#pragma endregion Implementation



#pragma region PublicMethods
void fro::AudioSDL::playMusic(std::string fileName, float const volume)
{
	m_pAudioSDLImplementation->playMusic(std::move(fileName), volume);
}

void fro::AudioSDL::playEffect(std::string fileName, float const volume)
{
	m_pAudioSDLImplementation->playEffect(std::move(fileName), volume);
}

void fro::AudioSDL::pauseMusic()
{
	m_pAudioSDLImplementation->pauseMusic();
}

void fro::AudioSDL::pauseEffect(std::string fileName)
{
	m_pAudioSDLImplementation->pauseEffect(std::move(fileName));
}

void fro::AudioSDL::pauseAllEffects()
{
	m_pAudioSDLImplementation->pauseAllEffects();
}

void fro::AudioSDL::stopMusic()
{
	m_pAudioSDLImplementation->stopMusic();
}

void fro::AudioSDL::stopEffect(std::string fileName)
{
	m_pAudioSDLImplementation->stopEffect(std::move(fileName));
}

void fro::AudioSDL::stopAllEffects()
{
	m_pAudioSDLImplementation->stopAllEffects();
}
#pragma endregion PublicMethods