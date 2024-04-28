#include "PlaySoundEffect.h"

#include "ServiceLocator.hpp"
#include "AudioService.h"

#pragma region Constructors/Destructor
fro::PlaySoundEffect::PlaySoundEffect(float const volume)
	: m_Volume{ volume }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::PlaySoundEffect::operator()()
{
	ServiceLocator<AudioService>::getInstance().getService().playEffect("Jingle #05.mp3", m_Volume);
}
#pragma endregion PublicMethods