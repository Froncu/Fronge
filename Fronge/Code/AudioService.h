#pragma once

#include <string>

namespace fro
{
	class AudioService
	{
	public:
		AudioService() = default;
		AudioService(AudioService const&) = delete;
		AudioService(AudioService&&) noexcept = delete;

		virtual ~AudioService() = default;

		AudioService& operator=(AudioService const&) = delete;
		AudioService& operator=(AudioService&&) noexcept = delete;

		virtual void update();

		virtual void playMusic(const std::string& fileName, float volume = 1.0f);
		virtual void pauseMusic();

		virtual void playEffect(const std::string& fileName, float volume = 1.0f);
		virtual void pauseEffect(const std::string& fileName);
		virtual void pauseAllEffects();
	};
}

#ifndef fro_GENERATED_AUDIO_SERVICE_BODY
#define fro_GENERATED_AUDIO_SERVICE_BODY(identifier)\
public:\
	identifier();\
\
	virtual ~identifier() override;\
\
	virtual void update();\
\
	virtual void playMusic(const std::string& fileName, float volume = 1.0f) override;\
	virtual void pauseMusic() override;\
\
	virtual void playEffect(const std::string& fileName, float volume = 1.0f) override;\
	virtual void pauseEffect(const std::string& fileName) override;\
	virtual void pauseAllEffects() override;
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_CONSTRUCTOR
#define fro_GENERATED_AUDIO_SERVICE_CONSTRUCTOR(identifier)\
fro::identifier::identifier()
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_DESTRUCTOR
#define fro_GENERATED_AUDIO_SERVICE_DESTRUCTOR(identifier)\
fro::identifier::~identifier()
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_UPDATE
#define fro_GENERATED_AUDIO_SERVICE_UPDATE(identifier)\
void fro::identifier::update()
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_PLAY_MUSIC
#define fro_GENERATED_AUDIO_SERVICE_PLAY_MUSIC(identifier)\
void fro::identifier::playMusic(const std::string& fileName, float volume)
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_PAUSE_MUSIC
#define fro_GENERATED_AUDIO_SERVICE_PAUSE_MUSIC(identifier)\
void fro::identifier::pauseMusic()
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_PLAY_EFFECT
#define fro_GENERATED_AUDIO_SERVICE_PLAY_EFFECT(identifier)\
void fro::identifier::playEffect(const std::string& fileName, float volume)
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_PAUSE_EFFECT
#define fro_GENERATED_AUDIO_SERVICE_PAUSE_EFFECT(identifier)\
void fro::identifier::pauseEffect(const std::string& fileName)
#endif

#ifndef fro_GENERATED_AUDIO_SERVICE_PAUSE_ALL_EFFECTS
#define fro_GENERATED_AUDIO_SERVICE_PAUSE_ALL_EFFECTS(identifier)\
void fro::identifier::pauseAllEffects()
#endif