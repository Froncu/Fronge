#pragma once

#include "AudioService.h"

#include <memory>

namespace fro
{
	class AudioSDL final : public AudioService
	{
		fro_GENERATED_AUDIO_SERVICE_BODY(AudioSDL)

	private:
		class AudioSDLImplementation;
		std::unique_ptr<AudioSDLImplementation> m_pAudioSDLImplementation;
	};
}