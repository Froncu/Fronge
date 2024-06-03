#include "AudioSDL.h"

#include "EventQueue.hpp"
#include "ResourceManager.h"

#include <sdl_mixer.h>

#include <condition_variable>
#include <format>
#include <map>
#include <stdexcept>
#include <thread>
#include <xstring>

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
	AudioSDLImplementation() = default;

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
					.play{ true }
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
					.play{ true }
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
					.isMusic{ true },
					.play{ false }
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
					.play{ false }
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
					.play{ false }
				});
		}

		m_ConditionVariable.notify_one();
	}

private:
	AudioSDLImplementation(AudioSDLImplementation const&) = delete;
	AudioSDLImplementation(AudioSDLImplementation&&) noexcept = delete;

	AudioSDLImplementation& operator=(AudioSDLImplementation const&) = delete;
	AudioSDLImplementation& operator=(AudioSDLImplementation&&) noexcept = delete;

	struct AudioEvent final
	{
	public:
		std::string fileName{};
		float volume{};
		bool isMusic{};
		bool play{};

		bool operator==(AudioEvent const& event) const
		{
			return
				this->fileName == event.fileName &&
				this->isMusic == event.isMusic &&
				this->play == event.play;
		}
	};

	bool m_RunThread{ true };
	std::mutex m_Mutex{};
	std::condition_variable m_ConditionVariable{};

	EventQueue<AudioEvent> m_EventQueue
	{
		[](AudioEvent&& event)
		{
			if (event.isMusic)
			{
				if (event.play)
				{
					Mix_VolumeMusic(static_cast<int>(event.volume * MIX_MAX_VOLUME));
					Mix_PlayMusic(ResourceManager::getInstance().getMusic(event.fileName), 0);
				}
				else
					Mix_PauseMusic();
			}
			else
			{
				static std::map<std::string, int> m_mEFFECT_CHANNELS{};

				if (event.play)
				{
					Mix_Chunk* const pEffect{ ResourceManager::getInstance().getEffect(event.fileName) };

					Mix_VolumeChunk(pEffect, static_cast<int>(event.volume * MIX_MAX_VOLUME));
					m_mEFFECT_CHANNELS[std::move(event.fileName)] = Mix_PlayChannel(-1, pEffect, 0);
				}
				else if (not event.fileName.empty())
				{
					auto const iterator{ m_mEFFECT_CHANNELS.find(event.fileName) };
					if (iterator == m_mEFFECT_CHANNELS.end())
						return;

					Mix_Pause(iterator->second);
				}
				else
					Mix_Pause(-1);
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
};
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
#pragma endregion PublicMethods