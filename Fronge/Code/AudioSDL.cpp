#include "AudioSDL.h"

#include "ResourceManager.h"
#include "EventQueue.hpp"

#include <SDL_mixer.h>
#include <stdexcept>
#include <format>
#include <string>
#include <map>
#include <thread>

#pragma region Constructors/Destructor
fro::AudioSDL::AudioSDL()
	: m_pAudioSDLImplementation{ std::make_unique<AudioSDLImplementation>() }
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
		throw std::runtime_error(std::format("[ Mix_OpenAudio() FAILED ] -> {}", Mix_GetError()));
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

	~AudioSDLImplementation() = default;

	void update()
	{
		m_EventProcessingThread = std::jthread(std::bind(&decltype(m_EventQueue)::processEvents, &m_EventQueue));
	}

	void playMusic(const std::string& fileName, float volume)
	{
		m_EventQueue.pushEvent(AudioEvent
			{
				.fileName{ fileName },
				.volume{ volume },
				.isMusic{ true },
				.play{ true }
			});
	}

	void playEffect(const std::string& fileName, float volume)
	{
		m_EventQueue.pushEvent(AudioEvent
			{
				.fileName{ fileName },
				.volume{ volume },
				.play{ true }
			});
	}

	void pauseMusic()
	{
		m_EventQueue.pushEvent(AudioEvent
			{
				.isMusic{ true },
				.play{ false }
			});
	}

	void pauseEffect(const std::string& fileName)
	{
		m_EventQueue.pushEvent(AudioEvent
			{
				.fileName{ fileName },
				.play{ false }
			});
	}

	void pauseAllEffects()
	{
		m_EventQueue.pushEvent(AudioEvent
			{
				.play{ false }
			});
	}

private:
	AudioSDLImplementation(AudioSDLImplementation&) = delete;
	AudioSDLImplementation(const AudioSDLImplementation&&) noexcept = delete;

	AudioSDLImplementation& operator=(AudioSDLImplementation&) = delete;
	AudioSDLImplementation& operator=(const AudioSDLImplementation&&) noexcept = delete;

	struct AudioEvent final
	{
	public:
		std::string fileName{};
		float volume{};
		bool isMusic{};
		bool play{};

		bool operator==(const AudioEvent& event) const
		{
			return
				this->fileName == event.fileName &&
				this->isMusic == event.isMusic &&
				this->play == event.play;
		}
	};

	std::jthread m_EventProcessingThread{};
	EventQueue<AudioEvent, std::function<void(AudioEvent&)>> m_EventQueue
	{
		[this](AudioEvent& event)
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
					m_mEFFECT_CHANNELS[event.fileName] = Mix_PlayChannel(-1, pEffect, 0);
				}
				else if (!event.fileName.empty())
				{
					const auto iterator{ m_mEFFECT_CHANNELS.find(event.fileName) };
					if (iterator == m_mEFFECT_CHANNELS.end())
						return;

					Mix_Pause(iterator->second);
				}
				else
					Mix_Pause(-1);
			}
		}
	};
};
#pragma endregion Implementation



#pragma region PublicMethods
void fro::AudioSDL::update()
{
	m_pAudioSDLImplementation->update();
}

void fro::AudioSDL::playMusic(const std::string& fileName, float volume)
{
	m_pAudioSDLImplementation->playMusic(fileName, volume);
}

void fro::AudioSDL::playEffect(const std::string& fileName, float volume)
{
	m_pAudioSDLImplementation->playEffect(fileName, volume);
}

void fro::AudioSDL::pauseMusic()
{
	m_pAudioSDLImplementation->pauseMusic();
}

void fro::AudioSDL::pauseEffect(const std::string& fileName)
{
	m_pAudioSDLImplementation->pauseEffect(fileName);
}

void fro::AudioSDL::pauseAllEffects()
{
	m_pAudioSDLImplementation->pauseAllEffects();
}
#pragma endregion PublicMethods